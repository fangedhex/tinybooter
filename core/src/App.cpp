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

void App::signalHandler(int signal, std::function<void(int)> exitCallback) {
  if (signal == SIGTERM) {
    spdlog::info("SIGTERM received. Quitting...");
  } else {
    spdlog::info("SIGINT received. Quitting...");
  }

  for(auto job : getJobs()) {
    job->stop();
  }

  exitCallback(EXIT_SUCCESS);
}

/**
 * @brief Parse cli arguments and loads configurations
 *
 * @param argc
 * @param argv
 */
void App::parseArgs(int argc, char **argv) {
  // Parse cli arguments with Cli11
  CLI::App app{"CHANGEME"};

  // Verbosity
  app.add_flag_function(
      "-v",
      [](int64_t count) {
        auto value = std::max((int64_t)(2 - count), (int64_t)(0));
        spdlog::set_level(static_cast<spdlog::level::level_enum>(value));
      },
      "Verbosity");

  std::string configFilePath = "/etc/tinybooter/config.yml";
  app.add_option("-c,--config-file", configFilePath, "Config file to use");

  app.parse(argc, argv);

  // Load configurations from the filesystem
  spdlog::debug("Loading global configuration from {} ...", configFilePath);
  YAML::Node node = YAML::LoadFile(configFilePath);
  this->systemConfig = node.as<SystemConfig>();

  // Monitor monitor(this->systemConfig.healthcheck_port);

  for (auto &p : glob::rglob(this->systemConfig.jobs)) {
    spdlog::debug("Loading job config from {} ...", p.generic_string());

    auto yaml = YAML::LoadFile(p.generic_string());

    if (!yaml["name"].IsDefined()) {
      // If name is not defined, we set the name to the filename
      yaml["name"] = p.stem().generic_string();
    }

    this->jobsConfig.push_back(yaml);
  }
}

void App::runOnce(JobKind kind) {
  spdlog::debug("Launching jobs with type {} ...", ToString(kind));
  for (auto &jobYaml : this->jobsConfig) {
    auto tmp = jobYaml.as<JobConfig>();
    if (tmp.kind != kind)
      continue;

    auto job = jobYaml.as<RunOnceJobConfig>();

    ChildProcess process(job.cmd, job.args);
    process.setSync(line_sink(job.name));
    if (!process.run()) {
      spdlog::error("Process didn't run");
    }
  }
}

void App::daemon() {
  spdlog::debug("Launching deamon jobs ...");
  for (auto &jobYaml : this->jobsConfig) {
    auto tmp = jobYaml.as<JobConfig>();
    if (tmp.kind != JobKind::SERVICE)
      continue;

    auto job = jobYaml.as<DaemonConfig>();

    ChildProcess process(job.cmd, job.args);
    process.setSync(line_sink(job.name));
    if (!process.run()) {
      spdlog::error("Process didn't run");
    }
  }
}

AppState App::getState() { return state; }

std::vector<Job *> App::getJobs() { return jobs; }

void App::addJob(Job *job) { jobs.push_back(job); }
