#include <iostream>
#include <vector>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>

#include "ConfigLoader.hpp"
#include "Service.hpp"
#include "shared.h"

bool healthcheck_run = true;
int sock;

void healthcheck_socket(std::vector<Service>& services)
{
    using std::cout, std::cerr, std::endl;

    if( (sock = socket(AF_UNIX, SOCK_STREAM, 0)) == -1 )
    {
        cerr << "Error creating socket for healthcheck" << endl;
        return;
    }

    {
        sockaddr_un addr{AF_UNIX, "/var/run/healthcheck.sock"};

        if( bind(sock, (sockaddr*)&addr, sizeof(addr)) == -1 )
        {
            cerr << "Error when binding unix socket" << endl;
            return;
        }
    }
    
    if( listen(sock, 5) == -1 )
    {
        cerr << "Error when setting up the queue" << endl;
        return;
    }

    while(healthcheck_run)
    {
        int client_sock;
        if( (client_sock = accept(sock, NULL, 0)) == -1 )
        {
            cerr << "Error when accepting new connection" << endl;
            healthcheck_run = false;
            break;
        }

        unsigned short count = 0;
        for(Service& service : services)
        {
            if(service.healthcheck()) count++;
        }

        if(count == 0) healthcheck_run = false;

        bool is_ok = (count == services.size());
        send(client_sock, &is_ok, sizeof(is_ok), 0);
        cout << "Sending healthcheck : " << is_ok << endl;
        close(client_sock);
    }
}

void interrupt(int signal)
{
    healthcheck_run = false;
    close(sock);
    exit(0);
}

void register_signal(int sig)
{
    if(SIG_ERR == signal(sig, interrupt)) 
    {
        std::cerr << "Cannot register signal !" << std::endl;
        exit(2);
    }
}

int main(int argc, char const *argv[])
{
    if(argc < 2) {
        std::cerr << "Syntax : " << argv[0] << " <config_dir>" << std::endl;
        return 1;
    }

    register_signal(SIGINT);
    register_signal(SIGTERM);

    auto logger = std::make_shared<Logger>();
    
    std::vector<Service> services;
    {
        // We load our configurations into services
        ConfigLoader loader;
        for(auto config : loader.getServicesConfigFromIni(argv[1]))
        {
            services.emplace_back(config, logger);
        }
    }

    // Waiting for them to stop
    for(auto& service : services)
    {
        service.launch();
    }

    // Launching healthcheck thread service   
    std::thread healthcheck_th(healthcheck_socket, std::ref(services));
    healthcheck_th.join();

    return 0;
}
