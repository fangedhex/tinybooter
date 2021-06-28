#include <ChildProcessMock.h>
#include <Job.h>
#include <config/RunOnceJobConfig.h>
#include <gtest/gtest.h>

using ::testing::Eq;
using ::testing::Return;

TEST(Job, RunOnce) {
  ChildProcessMock *childProcess = new ChildProcessMock();

  RunOnceJobConfig *config = new RunOnceJobConfig();
  config->kind = JobKind::INIT;
  config->name = "test";
  config->cmd = "echo";
  config->args = {"hello world!"};

  Job job(config, [childProcess](std::string cmd, Args args) {
    EXPECT_EQ(cmd, "echo");
    std::vector<std::string> expectedArgs{"hello world!"};
    EXPECT_THAT(args, Eq(expectedArgs));
    return childProcess;
  });

  EXPECT_CALL(*childProcess, run()).WillOnce(Return(true));

  job.launch();
  job.wait();

  EXPECT_EQ(job.getState(), JobState::SUCCESS);
}

TEST(Job, KeepRunning) {
  using namespace std::chrono_literals;
  using ::testing::Invoke;

  ChildProcessMock *childProcess = new ChildProcessMock();

  RunOnceJobConfig *config = new RunOnceJobConfig();
  config->kind = JobKind::SERVICE;
  config->name = "test";
  config->cmd = "sleep";
  config->args = {"5"};

  Job job(config, [childProcess](std::string cmd, Args args) {
    EXPECT_EQ(cmd, "sleep");
    std::vector<std::string> expectedArgs{"5"};
    EXPECT_THAT(args, Eq(expectedArgs));
    return childProcess;
  });

  EXPECT_CALL(*childProcess, run()).WillRepeatedly(Invoke([]() -> bool {
    std::this_thread::sleep_for(5000ms);
    return true;
  }));

  job.launch();

  std::this_thread::sleep_for(200ms);
  EXPECT_EQ(job.getState(), JobState::RUNNING);
  std::this_thread::sleep_for(5000ms);
  EXPECT_EQ(job.getState(), JobState::SUCCESS);

  job.stop();
  job.wait();
}
