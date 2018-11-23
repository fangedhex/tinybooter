/**
 * Structure that contains service's configuration loaded from ini file
 */

#pragma once

#include <string>

struct ServiceConfig
{
    std::string name;
    std::string command;
    bool restart;
    unsigned short max_retry;
};
