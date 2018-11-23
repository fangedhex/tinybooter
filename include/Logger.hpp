/**
 * Handle logging with multi threading
 */

#pragma once

#include <string>
#include <iostream>
#include <mutex>
#include <sstream>

class Logger
{
    std::mutex log_mutex;

    public:
    void log(std::string service_name, std::string str);  

    template<typename Arg, typename... Args>
    void log(std::string service_name, Arg&& arg, Args&&... args)
    {
        std::stringstream ss;
        ss << std::forward<Arg>(arg);
        using expander = int[];
        (void) expander{ (ss << std::forward<Args>(args), void(), 0)... };
        log(service_name, ss.str());
    }
};
