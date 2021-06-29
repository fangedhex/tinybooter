#include <JobMock.h>
#include <Monitor.h>
#include <gtest/gtest.h>
#include <httplib.h>

TEST(Monitor, shouldReturnCorrectStatus) {
  JobMock job;
  std::vector<Job *> jobs = {&job};

  Monitor monitor(8123, jobs);
  httplib::Client cli("localhost", 8123);

  auto startupRes = cli.Get("/startup");
  auto readinessRes = cli.Get("/readiness");
  auto livenessRes = cli.Get("/liveness");

  EXPECT_EQ(startupRes->status, 200);
  EXPECT_EQ(readinessRes->status, 200);
  EXPECT_EQ(livenessRes->status, 200);
}
