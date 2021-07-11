#pragma once

#include <Job.h>
#include <config/JobConfig.h>
#include <config/SystemConfig.h>
#include <Monitor.h>
#include <memory>
#include <vector>

enum AppState
{
  Init,
  Running,
  Cleanup
};

class App
{
public:
  virtual ~App();

  /// @brief Called by main function
  bool run(int argc, char **argv);

  /// @brief Shouldn't be called, main will call if we received SIGINT or SIGTERM
  void signalHandler(int signal, std::function<void(int)> exitCallback);

  /// @brief Current state of the application
  virtual AppState getState();

  /// @brief Returns the list of startup jobs
  virtual std::vector<Job *> getInitJobs();

  /// @brief Returns the list of daemon jobs
  virtual std::vector<Job *> getDaemonJobs();

  /// @brief Returns the list of clean jobs
  virtual std::vector<Job *> getCleanJobs();

private:
  AppState *state;
  Monitor *monitor;

  // TODO Do we need to keep those variables in runtime ?
  SystemConfig *systemConfig;
  std::vector<YAML::Node> jobsYaml;

  std::vector<Job *> initJobs;
  std::vector<Job *> daemonJobs;
  std::vector<Job *> cleanJobs;

  void parseArgsAndLoadConfiguration(int argc, char **argv);
  void createJobs();
  void runOnce(JobKind kind);
  void daemon();
};
