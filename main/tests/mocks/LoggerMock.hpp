#include "ILogger.hpp"
#include <gmock/gmock.h>

class LoggerMock : public ILogger
{
    public:
    MOCK_METHOD2(log, void(std::string, std::string));
};
