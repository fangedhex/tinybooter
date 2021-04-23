#pragma once

#include <memory>
#include <vector>

#include <config/SystemConfig.h>
#include <config/JobConfig.h>

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
  void runOnce(JobKind kind);
  void daemon();

private:
  SystemConfig systemConfig;
  std::vector<YAML::Node> jobsConfig;
  AppState currentState;
};
