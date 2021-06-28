#include <ChildProcess.h>
#include <gmock/gmock.h>

class ChildProcessMock : public ChildProcess {
public:
  MOCK_METHOD(bool, run, (), (override));
  MOCK_METHOD(int, stop, (), (override));
};
