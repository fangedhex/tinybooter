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
    std::shared_ptr<Logger> logger;
    std::vector<Service> &services;
    std::thread *th;
    int sock;
    void thread_func();
    bool keep_running;

    public:
    CliService(std::vector<Service> &services, std::shared_ptr<Logger> logger);
    ~CliService();
    void launch();
    void kill();
};
