#pragma once

#include <memory>
#include <SystemConfig.h>
#include <JobConfig.h>
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
  void signalHandler(int signal);
  void parseArgs(int argc, char **argv);

private:
  SystemConfig systemConfig;
  std::vector<JobConfig> jobsConfig;
  AppState currentState;
};
