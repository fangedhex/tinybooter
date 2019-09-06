#include <gtest/gtest.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "CliService.hpp"
#include "mocks/LoggerMock.hpp"

TEST(CliServiceTest, DISABLED_run)
{
    ServiceConfig config{
        "test",
        "sleep 0.1",
        false,
        0
    };

    LoggerMock logger;
    EXPECT_CALL(logger, log("cli", "Sending healthcheck : 0"));

    std::vector<Service> services;
    services.emplace_back(config, logger);    

    CliService service(3000, services, logger);
    service.launch();

    int cl_sock;
    if( (cl_sock = socket(AF_INET, SOCK_STREAM, 0)) == -1 )
    {
        FAIL();
        return;
    }

    {
        sockaddr_in addr{0};
        addr.sin_addr.s_addr = inet_addr("127.0.0.1");
        addr.sin_family = AF_INET;
        addr.sin_port = htons(3000);

        if( connect(cl_sock, (sockaddr*)&addr, sizeof(addr)) == -1 )
        {
            FAIL();
            return;
        }
    }

    bool is_ok;
    recv(cl_sock, &is_ok, sizeof(is_ok), 0);

    ASSERT_FALSE(is_ok);
}
