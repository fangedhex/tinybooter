#pragma once

#include <vector>
#include "Logger.hpp"
#include "CliService.hpp"

class Kernel
{
    Logger logger;
    CliService cliService;
    std::vector<Service> services;

    public:
    Kernel();
    int execute(int argc, const char** argv);
};
