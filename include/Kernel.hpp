#pragma once

#include <memory>
#include <vector>

#include "CliService.hpp"

class Kernel
{
    std::shared_ptr<Logger> logger;
    CliService cliService;
    std::vector<Service> services;

    public:
    Kernel();
    int execute(int argc, const char** argv);
};
