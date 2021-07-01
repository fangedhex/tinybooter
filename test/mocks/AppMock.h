#pragma once

#include <App.h>
#include <gmock/gmock.h>

class AppMock : public App {
public:
  MOCK_METHOD(AppState, getState, (), (override));
  MOCK_METHOD(std::vector<Job *>, getJobs, (), (override));
};
