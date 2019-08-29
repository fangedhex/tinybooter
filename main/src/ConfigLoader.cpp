#include "ConfigLoader.hpp"

#include <dirent.h>

#include "INIReader.h"

bool has_suffix(const std::string &str, const std::string &suffix)
{
    return str.size() >= suffix.size() && str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
}

std::vector<ServiceConfig> ConfigLoader::getServicesConfigFromIni(std::string path)
{
    DIR*    dir;
    dirent* pdir;
    std::vector<ServiceConfig> services;

    dir = opendir(path.c_str());

    while ((pdir = readdir(dir))) {
        std::string str = pdir->d_name;
        if(str != "." && str != "..")
        {
            if(has_suffix(str, ".ini"))
            {
                // Preparing path
                std::string sub_path = path;
                if(!has_suffix(path, "/")) sub_path += "/";
                sub_path += str;

                // Reading config
                INIReader reader(sub_path);
                if (reader.ParseError() >= 0) {
                    ServiceConfig serviceConfig{
                        reader.GetString("", "name", str),
                        reader.GetString("", "command", ""),
                        reader.GetBoolean("", "restart", true),
                        static_cast<unsigned short>(reader.GetInteger("", "max_retry", 5))
                    }; 

                    if(!serviceConfig.restart) serviceConfig.max_retry = 0;

                    services.push_back(serviceConfig);
                }
            }            
        }
    }

    closedir(dir);
    
    return services;    
}
