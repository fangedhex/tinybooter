/**
 * Each service will its own instance
 */

#pragma once

#include <iostream>
#include <thread>
#include <memory>

#include "ServiceConfig.hpp"
#include "Logger.hpp"

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
    std::shared_ptr<Logger> logger;

    void thread_func();    

    public:
    Service(ServiceConfig config, std::shared_ptr<Logger> logger);
    ~Service();
    void launch();
    bool healthcheck();
};
