#pragma once

#include <ChildProcess.h>
#include <config/JobConfig.h>
#include <functional>
#include <thread>

using Args = std::vector<std::string>;
using ChildProcessFactory =
    std::function<ChildProcess *(std::string cmd, Args args)>;

enum JobState { IDLE, STARTED, RUNNING, SUCCESS, FAILURE };

class Job {
public:
  Job(JobConfig *config, ChildProcessFactory childProcessFactory);
  ~Job();

  virtual void launch();
  virtual void stop();
  virtual void wait();

  virtual JobConfig getConfig() const;
  virtual JobState getState() const;

protected:
  Job();
  void internalRun();

private:
  JobState state;

  bool keepRunning;
  std::thread *thread;
  JobConfig *config;
  ChildProcess *childProcess;
};
