#include <gtest/gtest.h>
#include <JobsManager.h>
#include <factory/JobFactoryMock.h>

using ::testing::Return;

TEST(JobsManagerTest, Run)
{
  JobFactoryMock *jobFactory = new JobFactoryMock();
  JobsManager *jobsManager = new JobsManager(jobFactory);
  JobMock *job = new JobMock();

  EXPECT_CALL(*jobFactory, create()).Times(1).WillOnce(Return(job));

  YAML::Node jobNode;
  jobNode["kind"] = "init";
  jobNode["name"] = "test";
  jobNode["cmd"] = "echo";
  std::vector<std::string> args = {"hello world!"};
  jobNode["args"] = args;
  std::vector<YAML::Node> jobsYaml = {jobNode};

  jobsManager->createJobsFrom(jobsYaml);

  delete jobFactory;
  delete jobsManager;
}
