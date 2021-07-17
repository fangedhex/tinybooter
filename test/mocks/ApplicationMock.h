#pragma once

#include <Application.h>
#include <gmock/gmock.h>

class ApplicationMock : public Application {
public:
  ~ApplicationMock() {};
  MOCK_METHOD(AppState, getState, (), (override));
  MOCK_METHOD(std::vector<Job *>, getInitJobs, (), (override));
  MOCK_METHOD(std::vector<Job *>, getDaemonJobs, (), (override));
  MOCK_METHOD(std::vector<Job *>, getCleanJobs, (), (override));
};
