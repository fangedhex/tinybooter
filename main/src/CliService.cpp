#include "CliService.hpp"

#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <unistd.h>

CliService::CliService(std::vector<Service> &_s, ILogger& _l)
: logger(_l), services(_s)
{
}

CliService::CliService(std::string path, std::vector<Service> &_s, ILogger& _l)
: CliService(_s, _l)
{
    if( (sock = socket(AF_UNIX, SOCK_STREAM, 0)) == -1 )
    {
        logger.log("cli", "Error creating socket for healthcheck");
        return;
    }

    {
        sockaddr_un addr{AF_UNIX};
        strcpy(addr.sun_path, path.c_str());

        if( bind(sock, (sockaddr*)&addr, sizeof(addr)) == -1 )
        {
            logger.log("cli", "Error when binding unix socket");
            return;
        }
    }
    
    if( listen(sock, 5) == -1 )
    {
        logger.log("cli", "Error when setting up the queue");
        return;
    }
}

CliService::CliService(u_short port, std::vector<Service> &_s, ILogger& _l)
: CliService(_s, _l)
{
    if( (sock = socket(AF_INET, SOCK_STREAM, 0)) == -1 )
    {
        logger.log("cli", "Error creating socket for healthcheck");
        return;
    }

    {
        sockaddr_in addr{0};
        addr.sin_addr.s_addr = htonl(INADDR_ANY);
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);

        if( bind(sock, (sockaddr*)&addr, sizeof(addr)) == -1 )
        {
            logger.log("cli", "Error when binding tcp socket on port ", port);
            return;
        }
    }
    
    if( listen(sock, 5) == -1 )
    {
        logger.log("cli", "Error when setting up the queue");
        return;
    }
}

CliService::~CliService()
{
    kill();
    if(th)
    {
        th->join();
        delete th;
    }
    close(sock);
}

void CliService::thread_func()
{
    keep_running = true;
    while(keep_running)
    {
        int client_sock;
        if( (client_sock = accept(sock, NULL, 0)) == -1 )
        {
            if(keep_running) logger.log("cli", "Error when accepting new connection");
            keep_running = false;
            break;
        }

        unsigned short count = 0;        
        for(Service& service : services)
        {
            if(service.healthcheck()) count++;
        }        

        bool is_ok = (count == services.size());
        send(client_sock, &is_ok, sizeof(is_ok), 0);
        logger.log("cli", "Sending healthcheck : ", is_ok);
        close(client_sock);
    }
}

void CliService::launch()
{
    th = new std::thread(&CliService::thread_func, this);
}

void CliService::kill()
{
    keep_running = false;
    close(sock);
}