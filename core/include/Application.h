#pragma once

#include <Job.h>
#include <config/SystemConfig.h>
#include <JobsManager.h>
#include <Monitor.h>
#include <vector>

enum AppState
{
  Init,
  Running,
  Cleanup
};

class Application
{
public:
  Application();
  virtual ~Application();

  /// @brief Called by main function
  bool run(int argc, char **argv);

  /// @brief Shouldn't be called, main will call if we received SIGINT or SIGTERM
  void signalHandler(int signal, std::function<void(int)> exitCallback);

  /// @brief Current state of the application
  virtual AppState getState();

private:
  AppState *state;
  JobsManager *jobsManager;
  Monitor *monitor;

  // TODO Do we need to keep those variables in runtime ?
  SystemConfig *systemConfig;
  std::vector<YAML::Node> jobsYaml;

  void parseArgsAndLoadConfiguration(int argc, char **argv);
};
