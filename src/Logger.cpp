#include "Logger.hpp"

#include <chrono>
#include <iomanip>

void Logger::log(std::string service_name, std::string str)
{
    using std::cout, std::endl;

    if(str.size() > 1)
    {
        std::lock_guard<std::mutex> guard(log_mutex);
        auto NOW = std::chrono::system_clock::to_time_t( std::chrono::system_clock::now() );
        cout << "[" << std::put_time(std::localtime(&NOW), "%F %T") << "] ";
        cout << "[" << service_name << "] " << str << endl;
    }
}
