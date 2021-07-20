#pragma once

#include <Application.h>
#include <gmock/gmock.h>

class ApplicationMock : public Application {
public:
  MOCK_METHOD(AppState, getState, (), (override));
};
