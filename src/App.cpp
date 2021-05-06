#include <App.h>

#include <signal.h>
#include <spdlog/spdlog.h>
#include <CLI/App.hpp>
#include <CLI/Formatter.hpp>
#include <CLI/Config.hpp>
#include <yaml-cpp/yaml.h>
#include <util/glob.h>
#include <config/RunOnceJobConfig.h>
#include <SyncProcess.h>
#include <config/DaemonConfig.h>
#include <AsyncProcess.h>
#include <exception>

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
  spdlog::set_level(spdlog::level::debug);

  // Parse cli arguments with Cli11
  CLI::App app{"CHANGEME"};

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
    this->jobsConfig.push_back(YAML::LoadFile(p));
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

    SyncProcess process(job.cmd, job.args);
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

    AsyncProcess process(job.cmd, job.args);
    if (!process.run())
    {
      spdlog::error("Process didn't run");
    }
  }
}
