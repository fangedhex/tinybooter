#pragma once

#include <string>
#include <sstream>

class ILogger
{
    public:
    virtual void log(std::string service_name, std::string str) = 0;  

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
