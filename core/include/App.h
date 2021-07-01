#pragma once

#include <Job.h>
#include <config/JobConfig.h>
#include <config/SystemConfig.h>
#include <memory>
#include <vector>

enum AppState { Init, Running, Cleanup };

class App {
public:
  void signalHandler(int signal);

  void parseArgs(int argc, char **argv);
  void runOnce(JobKind kind);
  void daemon();

  virtual AppState getState();
  virtual std::vector<Job *> getJobs();

private:
  AppState state;
  std::vector<Job *> jobs;
  SystemConfig systemConfig;
  std::vector<YAML::Node> jobsConfig;
  AppState currentState;
};
