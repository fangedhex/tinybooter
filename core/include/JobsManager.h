#pragma once

#include <vector>
#include <yaml-cpp/yaml.h>
#include <util/BasicFactory.h>
#include <Job.h>

using JobFactory = BasicFactory<Job>;

class JobsManager
{
  public:
  JobsManager(JobFactory *jobFactory);
  virtual ~JobsManager();

  /// @brief Create jobs from the configurations given as argument.
  void createJobsFrom(std::vector<YAML::Node> jobsYaml);

  /// @brief Run jobs of the specified kind
  void runOnce(JobKind kind);

  /// @brief Run jobs of the specified kind and keep them alive
  void daemon();

  /// @brief Will stop and wait all currently running jobs
  void stopAll();

  /// @brief Will wait for all currently running daemon jobs to finish
  void waitAllDaemonJobsToFinish();

  /// @brief Returns the list of startup jobs
  virtual std::vector<Job *> getInitJobs();

  /// @brief Returns the list of daemon jobs
  virtual std::vector<Job *> getDaemonJobs();

  /// @brief Returns the list of clean jobs
  virtual std::vector<Job *> getCleanJobs();

  private:
  BasicFactory<Job> *jobFactory;

  std::vector<Job *> initJobs;
  std::vector<Job *> daemonJobs;
  std::vector<Job *> cleanJobs;
};
