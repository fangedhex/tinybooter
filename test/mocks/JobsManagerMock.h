#pragma once

#include <gmock/gmock.h>
#include <JobsManager.h>

class JobsManagerMock : public JobsManager
{
  public:
  JobsManagerMock() : JobsManager(nullptr) {};

  MOCK_METHOD(std::vector<Job *>, getInitJobs, (), (override));
  MOCK_METHOD(std::vector<Job *>, getDaemonJobs, (), (override));
  MOCK_METHOD(std::vector<Job *>, getCleanJobs, (), (override));
};
