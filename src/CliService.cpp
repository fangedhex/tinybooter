#include "CliService.hpp"

#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

CliService::CliService(std::vector<Service> &_s, Logger& _l)
: logger(_l), services(_s)
{
}

CliService::~CliService()
{
    kill();
    if(th)
    {
        th->join();
        delete th;
    }
}

void CliService::thread_func()
{
    if( (sock = socket(AF_UNIX, SOCK_STREAM, 0)) == -1 )
    {
        logger.log("cli", "Error creating socket for healthcheck");
        return;
    }

    {
        sockaddr_un addr{AF_UNIX, "/var/run/healthcheck.sock"};

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
