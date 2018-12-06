#pragma once

#include <vector>

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
