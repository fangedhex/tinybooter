#pragma once

#include <ChildProcess.h>
#include <config/JobConfig.h>
#include <functional>

using ChildProcessFactory = std::function<ChildProcess *()>;

class Job {
public:
  Job(JobConfig config, ChildProcessFactory childProcessFactory);

private:
  ChildProcess *childProcess;
};
