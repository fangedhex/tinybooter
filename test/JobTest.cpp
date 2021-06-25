#include <ChildProcessMock.h>
#include <Job.h>
#include <gtest/gtest.h>

TEST(Job, Test) {
  using ::testing::NiceMock;

  ChildProcessMock childProcess;
  JobConfig config;

  Job job(config, [&childProcess]() { return &childProcess; });
}
