/**
 * Unix socket client that connect to tinybooter to know if the services are running or not
 */
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "shared.h"

int main(int argc, char const *argv[])
{
    int sock = socket(AF_UNIX, SOCK_STREAM, 0);    

    {
        // We are connecting to a local UNIX socket
        sockaddr_un addr{AF_UNIX, "/var/run/healthcheck.sock"};
        connect(sock, (sockaddr*)&addr, sizeof(addr));
    }    

    // We read our only byte as a boolean that tinybooter send to us.
    bool is_ok = false;
    recv(sock, &is_ok, sizeof(is_ok), 0);

    return !is_ok;
}
