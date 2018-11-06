#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#define UNIX_PATH_MAX 108

int main(int argc, char const *argv[])
{
    int sock = socket(AF_UNIX, SOCK_STREAM, 0);

    struct sockaddr_un {
        sa_family_t sun_family;               /* AF_UNIX */
        char        sun_path[UNIX_PATH_MAX];  /* pathname */
    };

    sockaddr_un addr{AF_UNIX, "/var/run/healthcheck.sock"};

    connect(sock, (sockaddr*)&addr, sizeof(addr));

    bool is_ok = false;
    recv(sock, &is_ok, sizeof(is_ok), 0);

    return !is_ok;
}
