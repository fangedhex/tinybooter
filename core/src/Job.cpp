#include <Job.h>

Job::Job(JobConfig config, ChildProcessFactory childProcessFactory) {
  this->childProcess = childProcessFactory();
}
