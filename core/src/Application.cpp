#include <Application.h>

#include <CLI/App.hpp>
#include <CLI/Config.hpp>
#include <CLI/Formatter.hpp>
#include <ChildProcess.h>
#include <algorithm>
#include <exception>
#include <glob/glob.h>
#include <signal.h>
#include <spdlog/spdlog.h>
#include <yaml-cpp/yaml.h>

Application::Application()
: jobsManager(new JobsManager(new BasicFactory<Job>())) {}

Application::~Application()
{
  if (state != nullptr)
    delete state;

  if (systemConfig != nullptr)
    delete systemConfig;

  if (jobsManager != nullptr)
    delete jobsManager;

  if (monitor != nullptr)
    delete monitor;
}

AppState Application::getState() { return *this->state; }

bool Application::run(int argc, char **argv)
{
  *state = AppState::Init;

  // Parse command line arguments + read configuration files
  try
  {
    // TODO Maybe this function should return the yaml instead of putting it as member of the class?
    this->parseArgsAndLoadConfiguration(argc, argv);
  }
  catch (const std::exception &e)
  {
    spdlog::error(e.what());
    return EXIT_FAILURE;
  }

  // Launch monitor server
  monitor = new Monitor(this->systemConfig->healthcheck_port, this, this->jobsManager);

  // Create jobs from load configuration files
  this->jobsManager->createJobsFrom(this->jobsYaml);

  // Launch init jobs
  this->jobsManager->runOnce(JobKind::INIT);

  // Launch daemon jobs
  this->jobsManager->daemon();

  // Wait for daemon jobs to exit
  this->jobsManager->waitAllDaemonJobsToFinish();

  // Launch cleanup jobs
  this->jobsManager->runOnce(JobKind::CLEANUP);

  return EXIT_SUCCESS;
}

void Application::parseArgsAndLoadConfiguration(int argc, char **argv)
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

void Application::signalHandler(int signal, std::function<void(int)> exitCallback)
{
  if (signal == SIGTERM)
  {
    spdlog::info("SIGTERM received. Quitting...");
  }
  else
  {
    spdlog::info("SIGINT received. Quitting...");
  }

  this->jobsManager->stopAll();

  exitCallback(EXIT_SUCCESS);
}
