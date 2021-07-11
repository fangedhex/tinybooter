#include <gtest/gtest.h>
#include <App.h>
#include <JobMock.h>
#include <csignal>

/*TEST(App, ShouldStopJobs) {
  JobMock job;
  App app;
  app.addJob(&job);

  EXPECT_CALL(job, stop()).Times(1);

  app.signalHandler(SIGTERM, [](int status) {
    EXPECT_EQ(status, EXIT_SUCCESS);
  });
}*/
