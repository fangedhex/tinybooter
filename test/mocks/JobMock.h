#pragma once

#include <Job.h>
#include <gmock/gmock.h>

class JobMock : public Job {
public:
  MOCK_METHOD(void, launch, (), (override));
  MOCK_METHOD(void, stop, (), (override));
  MOCK_METHOD(void, wait, (), (override));
  MOCK_METHOD(JobState, getState, (), (override, const));
};
