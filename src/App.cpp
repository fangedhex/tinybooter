#include <App.h>

#include <signal.h>
#include <spdlog/spdlog.h>
#include <CLI/App.hpp>
#include <CLI/Formatter.hpp>
#include <CLI/Config.hpp>
#include <yaml-cpp/yaml.h>
#include <util/glob.h>
#include <config/RunOnceJobConfig.h>
#include <config/DaemonConfig.h>
#include <ChildProcess.h>
#include <exception>
#include <algorithm>

void App::signalHandler(int signal)
{
  if (signal == SIGTERM)
  {
    spdlog::info("SIGTERM received. Quitting...");
  }
  else
  {
    spdlog::info("SIGINT received. Quitting...");
  }

  // TODO Code stuff to stop current running jobs

  exit(0);
}

/**
 * @brief Parse cli arguments and loads configurations
 *
 * @param argc
 * @param argv
 */
void App::parseArgs(int argc, char **argv)
{
  // Parse cli arguments with Cli11
  CLI::App app{"CHANGEME"};

  // Verbosity
  app.add_flag_function("-v", [](int64_t count) {
    auto value = std::max((int64_t)(2 - count), (int64_t)(0));
    spdlog::set_level(static_cast<spdlog::level::level_enum>(value));
  }, "Verbosity");

  std::string configFilePath = "/etc/tinybooter/config.yml";
  app.add_option("-c,--config-file", configFilePath, "Config file to use");

  app.parse(argc, argv);

  // Load configurations from the filesystem
  spdlog::debug("Loading global configuration from {} ...", configFilePath);
  YAML::Node node = YAML::LoadFile(configFilePath);
  this->systemConfig = node.as<SystemConfig>();

  //Monitor monitor(this->systemConfig.healthcheck_port);

  for (auto &p : glob::rglob(this->systemConfig.jobs))
  {
    spdlog::debug("Loading job config from {} ...", p.c_str());

    auto yaml = YAML::LoadFile(p);

    if (!yaml["name"].IsDefined()) {
      // If name is not defined, we set the name to the filename
      yaml["name"] = p.stem().generic_string();
    }

    this->jobsConfig.push_back(yaml);
  }
}

void App::runOnce(JobKind kind)
{
  spdlog::debug("Launching jobs with type {} ...", ToString(kind));
  for (auto &jobYaml : this->jobsConfig)
  {
    auto tmp = jobYaml.as<JobConfig>();
    if (tmp.kind != kind)
      continue;

    auto job = jobYaml.as<RunOnceJobConfig>();

    ChildProcess process(job.cmd, job.args);
    process.setSync(line_sink(job.name));
    if (!process.run())
    {
      spdlog::error("Process didn't run");
    }
  }
}

void App::daemon()
{
  spdlog::debug("Launching deamon jobs ...");
  for (auto &jobYaml : this->jobsConfig)
  {
    auto tmp = jobYaml.as<JobConfig>();
    if (tmp.kind != JobKind::SERVICE)
      continue;

    auto job = jobYaml.as<DaemonConfig>();

    ChildProcess process(job.cmd, job.args);
    process.setSync(line_sink(job.name));
    if (!process.run())
    {
      spdlog::error("Process didn't run");
    }
  }
}
