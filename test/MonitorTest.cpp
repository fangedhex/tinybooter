#include <ApplicationMock.h>
#include <JobsManagerMock.h>
#include <JobMock.h>
#include <Monitor.h>
#include <gtest/gtest.h>
#include <httplib.h>

using ::testing::Return;

TEST(Monitor, Startup) {
  JobMock* job = new JobMock();
  std::vector<Job *> jobs = {job};

  ApplicationMock* app = new ApplicationMock();
  JobsManagerMock* jobsManager = new JobsManagerMock();

  JobConfig config;
  config.kind = JobKind::INIT;

  Monitor monitor(8123, app, jobsManager);

  httplib::Client cli("localhost", 8123);

  {
    EXPECT_CALL(*app, getState).Times(3).WillRepeatedly(Return(AppState::Init));
    EXPECT_CALL(*jobsManager, getInitJobs).Times(2).WillRepeatedly(Return(jobs));

    EXPECT_CALL(*job, getConfig).Times(2).WillRepeatedly(Return(config));
    EXPECT_CALL(*job, getState)
        .Times(2)
        .WillRepeatedly(Return(JobState::RUNNING));

    auto startupRes = cli.Get("/startup");
    auto readinessRes = cli.Get("/readiness");
    auto livenessRes = cli.Get("/liveness");

    EXPECT_EQ(startupRes->status, 500);
    EXPECT_EQ(readinessRes->status, 500);
    EXPECT_EQ(livenessRes->status, 500);
  }

  {
    EXPECT_CALL(*app, getState).Times(3).WillRepeatedly(Return(AppState::Init));
    EXPECT_CALL(*jobsManager, getInitJobs).Times(2).WillRepeatedly(Return(jobs));

    EXPECT_CALL(*job, getConfig).Times(2).WillRepeatedly(Return(config));
    EXPECT_CALL(*job, getState)
        .Times(2)
        .WillRepeatedly(Return(JobState::SUCCESS));

    auto startupRes = cli.Get("/startup");
    auto readinessRes = cli.Get("/readiness");
    auto livenessRes = cli.Get("/liveness");

    EXPECT_EQ(startupRes->status, 200);
    EXPECT_EQ(readinessRes->status, 200);
    EXPECT_EQ(livenessRes->status, 500);
  }

  delete jobsManager;
  delete app;
  delete job;
}
