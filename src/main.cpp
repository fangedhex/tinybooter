#include <iostream>
#include <dirent.h>
#include <vector>
#include <string>

bool has_suffix(const std::string &str, const std::string &suffix)
{
    return str.size() >= suffix.size() && str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
}

std::vector<std::string> open(std::string path = ".") 
{
    DIR*    dir;
    dirent* pdir;
    std::vector<std::string> files;

    dir = opendir(path.c_str());

    while ((pdir = readdir(dir))) {
        std::string str = pdir->d_name;
        if(str != "." && str != "..")
        {
            if(has_suffix(str, ".ini"))
            {
                std::string sub_path = path;
                if(!has_suffix(path, "/")) sub_path += "/";
                sub_path += str;
                files.push_back(sub_path);
            }            
        }
    }

    closedir(dir);
    
    return files;
}

#include "INIReader.h"

int main(int argc, char const *argv[])
{
    using std::cout, std::endl;

    auto files = open();
    for(auto file : files)
    {
        cout << file << endl;

        INIReader reader(file);

        if (reader.ParseError() < 0) {
            std::cout << "Can't load 'test.ini'\n";
            return 1;
        }

        cout << "Command = " << reader.GetString("", "command", "") << endl;
    }

    return 0;
}
