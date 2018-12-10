#include <gtest/gtest.h>
#include "LoggerMock.hpp"
#include "Service.hpp"

TEST(ServiceTest, launchOnce)
{
    testing::Sequence seq;

    ServiceConfig config{
        "test",
        "sleep 1",
        false,
        0
    };

    LoggerMock logger;
    EXPECT_CALL(logger, log("test", "Starting ...")).Times(1).InSequence(seq);
    EXPECT_CALL(logger, log("test", "Service executed once as expected.")).Times(1).InSequence(seq);

    Service service(config, logger);
    service.launch();
}
