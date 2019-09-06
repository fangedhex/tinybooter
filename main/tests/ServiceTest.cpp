#include <gtest/gtest.h>
#include "mocks/LoggerMock.hpp"
#include "Service.hpp"

TEST(ServiceTest, launchOnce)
{
    testing::Sequence seq;

    ServiceConfig config{
        "test",
        "echo 'test'",
        false,
        0
    };

    LoggerMock logger;
    EXPECT_CALL(logger, log("test", "Starting ...")).Times(1).InSequence(seq);
    EXPECT_CALL(logger, log("test", "Service executed once as expected.")).Times(1).InSequence(seq);

    Service service(config, logger);
    service.launch();

    EXPECT_TRUE(service.healthcheck());
}
