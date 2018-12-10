/**
 * Handle unix socket for cli like connections
 */

#pragma once

#include <vector>
#include <thread>

#include "Service.hpp"
#include "shared.h"

class CliService
{
    ILogger& logger;
    std::vector<Service> &services;
    std::thread *th;
    int sock;
    void thread_func();
    bool keep_running;

    public:
    CliService(std::vector<Service> &services, ILogger& logger);
    ~CliService();
    void launch();
    void kill();
};
