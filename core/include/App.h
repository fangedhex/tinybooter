#pragma once

#include <Job.h>
#include <config/JobConfig.h>
#include <config/SystemConfig.h>
#include <memory>
#include <vector>

enum AppState { Init, Running, Cleanup };

class App {
public:
  /// @brief Shouldn't be called, main will call if we received SIGINT or SIGTERM
  void signalHandler(int signal, std::function<void(int)> exitCallback);

  /// @brief Called by main with arguments given by the shell
  void parseArgs(int argc, char **argv);

  /// @brief Current state of the application
  virtual AppState getState();

  /// @brief Returns current loaded jobs
  virtual std::vector<Job *> getJobs();

  /// @brief Adds a job to the current loaded jobs list
  void addJob(Job * job);

  void runOnce(JobKind kind);
  void daemon();

private:
  AppState state;
  std::vector<Job *> jobs;
  SystemConfig systemConfig;
  std::vector<YAML::Node> jobsConfig;
  AppState currentState;
};
