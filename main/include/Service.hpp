/**
 * Each service will its own instance
 */

#pragma once

#include <iostream>
#include <thread>

#include "ServiceConfig.hpp"
#include "ILogger.hpp"

enum ServiceFlag
{
    RUNNING = 1,
    RUNNED_ONCE = 2,
    KEEP_RUNNING = 4
};

class Service
{
    ServiceConfig config;
    std::thread *thread = nullptr;    
    char flags = 0;
    ILogger& logger;

    void thread_func();    

    public:
    Service(ServiceConfig config, ILogger& logger);
    ~Service();
    void launch();
    bool healthcheck();
    bool is_running();
};
