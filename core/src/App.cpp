#include <App.h>

#include <CLI/App.hpp>
#include <CLI/Config.hpp>
#include <CLI/Formatter.hpp>
#include <ChildProcess.h>
#include <algorithm>
#include <config/DaemonConfig.h>
#include <config/RunOnceJobConfig.h>
#include <exception>
#include <glob/glob.h>
#include <signal.h>
#include <spdlog/spdlog.h>
#include <yaml-cpp/yaml.h>

App::~App()
{
  if (state != nullptr)
    delete state;

  if (systemConfig != nullptr)
    delete systemConfig;

  if (monitor != nullptr)
    delete monitor;

  for (auto job : this->initJobs)
  {
    delete job;
  }
  for (auto job : this->daemonJobs)
  {
    delete job;
  }
  for (auto job : this->cleanJobs)
  {
    delete job;
  }
}

AppState App::getState() { return *this->state; }

std::vector<Job *> App::getInitJobs() { return this->initJobs; }

std::vector<Job *> App::getDaemonJobs() { return this->daemonJobs; }

std::vector<Job *> App::getCleanJobs() { return this->cleanJobs; }

bool App::run(int argc, char **argv)
{
  *state = AppState::Init;

  // Parse command line arguments + read configuration files
  try
  {
    this->parseArgsAndLoadConfiguration(argc, argv);
  }
  catch (const std::exception &e)
  {
    spdlog::error(e.what());
    return EXIT_FAILURE;
  }

  // Launch monitor server
  monitor = new Monitor(this->systemConfig->healthcheck_port, this);

  // Create jobs from load configuration files
  this->createJobs();

  // Launch init jobs
  this->runOnce(JobKind::INIT);

  // Launch daemon jobs
  this->daemon();

  // Wait for daemon jobs to exit (the only way is by receiving a signal SIGTERM
  // or SIGINT)

  // Launch cleanup jobs
  this->runOnce(JobKind::CLEANUP);

  return EXIT_SUCCESS;
}

void App::parseArgsAndLoadConfiguration(int argc, char **argv)
{
  std::string configFilePath = "/etc/tinybooter/config.yml";
  {
    // Parse cli arguments with Cli11
    CLI::App app{
        "Small helper to boot up jobs / programs inside of a container."};

    // Verbosity
    app.add_flag_function(
        "-v",
        [](int64_t count)
        {
          auto value = std::max((int64_t)(2 - count), (int64_t)(0));
          spdlog::set_level(static_cast<spdlog::level::level_enum>(value));
        },
        "Verbosity");

    app.add_option("-c,--config-file", configFilePath, "Config file to use");

    try
    {
      app.parse(argc, argv);
    }
    catch (const CLI::ParseError &e)
    {
      app.exit(e);
      throw std::runtime_error("Couldn't parse arguments");
    }
  }

  // Load configurations from the filesystem
  spdlog::debug("Loading global configuration from {} ...", configFilePath);
  YAML::Node node = YAML::LoadFile(configFilePath);
  this->systemConfig = node.as<SystemConfig *>();

  for (auto &p : glob::rglob(this->systemConfig->jobs))
  {
    spdlog::debug("Loading job config from {} ...", p.generic_string());

    auto yaml = YAML::LoadFile(p.generic_string());

    if (!yaml["name"].IsDefined())
    {
      // If name is not defined, we set the name to the filename
      yaml["name"] = p.stem().generic_string();
    }

    this->jobsYaml.push_back(yaml);
  }
}

void App::createJobs() {
  for(auto &jobYaml : this->jobsYaml)
  {
    // Create specialized job configuration depending on the job's kind
    auto kind = jobYaml["kind"].as<JobKind>();
    JobConfig* config;

    if (kind == JobKind::SERVICE)
    {
      config = jobYaml.as<DaemonConfig*>();
    }
    else
    {
      config = jobYaml.as<RunOnceJobConfig*>();
    }

    // TODO Maybe remove the needs of a factory function and just build and pass the childprocess directly ?
    auto childProcessFactory = [config](std::string cmd, Args args) -> ChildProcess* {
      auto childProcess = new ChildProcess(cmd, args);
      childProcess->setSync(line_sink(config->name));
      return childProcess;
    };

    auto job = new Job(config, childProcessFactory);

    switch(kind)
    {
      case JobKind::INIT:
        this->initJobs.push_back(job);
        break;
      case JobKind::SERVICE:
        this->daemonJobs.push_back(job);
        break;
      case JobKind::CLEANUP:
        this->cleanJobs.push_back(job);
        break;
    }
  }
}

void App::runOnce(JobKind kind)
{
  spdlog::debug("Launching jobs with type {} ...", ToString(kind));

  std::vector<Job*> jobs;
  if (kind == JobKind::INIT) {
    jobs = this->initJobs;
  } else {
    jobs = this->cleanJobs;
  }

  for (auto job : jobs)
  {
    job->launch();
    job->wait();
  }
}

void App::daemon()
{
  spdlog::debug("Launching deamon jobs ...");
  for (auto job : this->daemonJobs)
  {
    job->launch();
  }

  // We will wait until all daemon jobs exited
  for(auto job : this->daemonJobs) {
    job->wait();
  }
}

void App::signalHandler(int signal, std::function<void(int)> exitCallback)
{
  if (signal == SIGTERM)
  {
    spdlog::info("SIGTERM received. Quitting...");
  }
  else
  {
    spdlog::info("SIGINT received. Quitting...");
  }

  for (auto job : this->initJobs)
  {
    job->stop();
    job->wait();
  }

  for (auto job : this->daemonJobs)
  {
    job->stop();
    job->wait();
  }

  exitCallback(EXIT_SUCCESS);
}
