/**
 * Class that manage & load configurations from ini files
 */

#pragma once

#include <vector>

#include "ServiceConfig.hpp"

class ConfigLoader
{
    public:
    std::vector<ServiceConfig> getServicesConfigFromIni(std::string path);
};
