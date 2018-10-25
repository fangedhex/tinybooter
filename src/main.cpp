#include <iostream>
#include <dirent.h>
#include <vector>
#include <string>

#include <thread>

struct Service
{
    std::string name;
    std::string command;
    bool restart;
    unsigned short max_retry;

    std::thread *pid;
};


bool has_suffix(const std::string &str, const std::string &suffix)
{
    return str.size() >= suffix.size() && str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
}

#include "INIReader.h"
#define CONFIG_DIR "./tinybooter.d"

std::vector<Service> loadConfig() 
{
    DIR*    dir;
    dirent* pdir;
    std::vector<Service> services;

    dir = opendir(CONFIG_DIR);

    while ((pdir = readdir(dir))) {
        std::string str = pdir->d_name;
        if(str != "." && str != "..")
        {
            if(has_suffix(str, ".ini"))
            {
                // Preparing path
                std::string sub_path = CONFIG_DIR;
                if(!has_suffix(CONFIG_DIR, "/")) sub_path += "/";
                sub_path += str;

                // Reading config
                INIReader reader(sub_path);
                if (reader.ParseError() >= 0) {
                    Service service{
                        reader.GetString("", "name", str),
                        reader.GetString("", "command", ""),
                        reader.GetBoolean("", "restart", true),
                        static_cast<unsigned short>(reader.GetInteger("", "max_retry", 5))
                    }; 
                    services.push_back(service);
                }
            }            
        }
    }

    closedir(dir);
    
    return services;
}

std::string read_line(FILE *stream)
{
    std::string str;

    while(!feof(stream))
    {
        int c;
        if((c = fgetc(stream)))
        {
            if(c == '\n') return str;
            str += c;
        }
    }

    return str;
}

void service_log(Service& service, std::string str)
{
    using std::cout, std::endl;

    if(str.size() > 1)
    {
        cout << "[" << service.name << "] " << str << endl;
    }
}

#include <sstream>

template<typename Arg, typename... Args>
void service_log(Service& service, Arg&& arg, Args&&... args)
{
    std::stringstream ss;
    ss << std::forward<Arg>(arg);

    using expander = int[];
    (void) expander{ (ss << std::forward<Args>(args), void(), 0)... };

    service_log(service, ss.str());
}

void launch_service(Service& service)
{
    unsigned short retry = 0;
    do
    {
        service_log(service, "Starting...");
        auto stream = popen((service.command + " 2>&1").c_str(), "r");

        if(stream)
        {
            while(!feof(stream))
            {
                service_log(service, read_line(stream));
            }

            if(pclose(stream) != 0)
            {
                retry++;
                if(retry >= service.max_retry) service.restart = false;
            }
        }   
    }while(service.restart);

    if(service.max_retry > 0)
    {
        service_log(service, "Service didn't start after ", service.max_retry, " retries.");
    }    
}

int main(int argc, char const *argv[])
{
    auto services = loadConfig();

    // Launching each service in a separate thread
    for(auto& service : services)
    {
        service.pid = new std::thread(launch_service, std::ref(service));
    }

    // Waiting for each thread to shutdown
    for(auto& service : services)
    {
        service.pid->join();
        delete service.pid;
    }

    return 0;
}
