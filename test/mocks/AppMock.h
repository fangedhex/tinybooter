#pragma once

#include <App.h>
#include <gmock/gmock.h>

class AppMock : public App {
public:
  ~AppMock() {};
  MOCK_METHOD(AppState, getState, (), (override));
  MOCK_METHOD(std::vector<Job *>, getInitJobs, (), (override));
  MOCK_METHOD(std::vector<Job *>, getDaemonJobs, (), (override));
  MOCK_METHOD(std::vector<Job *>, getCleanJobs, (), (override));
};
