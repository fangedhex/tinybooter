/**
 * Handle logging with multi threading
 */

#pragma once

#include "ILogger.hpp"
#include <mutex>

class Logger : public ILogger
{
    std::mutex log_mutex;

    public:
    void log(std::string service_name, std::string str);
};
