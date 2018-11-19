#include <iostream>
#include <dirent.h>
#include <vector>
#include <string>
#include <thread>
#include <chrono>
#include <iomanip>
#include <mutex>
#include <sstream>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>

#include "shared.h"
#include "INIReader.h"

struct Service
{
    std::string name;
    std::string command;
    bool restart;
    unsigned short max_retry;

    std::thread *pid = nullptr;
    bool is_running = false;
};

bool has_suffix(const std::string &str, const std::string &suffix)
{
    return str.size() >= suffix.size() && str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
}

std::vector<Service> loadConfig(std::string path) 
{
    DIR*    dir;
    dirent* pdir;
    std::vector<Service> services;

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
                    Service service{
                        reader.GetString("", "name", str),
                        reader.GetString("", "command", ""),
                        reader.GetBoolean("", "restart", true),
                        static_cast<unsigned short>(reader.GetInteger("", "max_retry", 5))
                    }; 

                    if(!service.restart) service.max_retry = 0;

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

std::mutex log_mutex;
void service_log(Service& service, std::string str)
{
    using std::cout, std::endl;    

    if(str.size() > 1)
    {
        std::lock_guard<std::mutex> guard(log_mutex);
        auto NOW = std::chrono::system_clock::to_time_t( std::chrono::system_clock::now() );
        cout << "[" << std::put_time(std::localtime(&NOW), "%F %T") << "] ";
        cout << "[" << service.name << "] " << str << endl;
    }
}

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
    bool keep_running = true;
    
    do
    {
        service_log(service, "Starting...");
        auto stream = popen((service.command + " 2>&1").c_str(), "r");

        if(stream)
        {
            service.is_running = true;
            while(!feof(stream))
            {
                service_log(service, read_line(stream));
            }

            if(pclose(stream) != 0)
            {
                // We increment retry value and if it exceeds our max retry value, we stop our loop
                retry++;
                if(retry >= service.max_retry) keep_running = false;
            }
            
            if(service.restart) service.is_running = false;
        }   
    }
    while(keep_running && service.restart);

    if(service.restart)
    {
        // We notify that we can't run the service if the service is meant to restart
        service_log(service, "Service didn't start after ", service.max_retry, " retries.");
    }    
}

bool healthcheck_run = true;
int sock;

void healthcheck_socket(std::vector<Service>& services)
{
    sock = socket(AF_UNIX, SOCK_STREAM, 0);

    {
        sockaddr_un addr{AF_UNIX, "/var/run/healthcheck.sock"};
        bind(sock, (sockaddr*)&addr, sizeof(addr));
    }
    
    listen(sock, 5);

    while(healthcheck_run)
    {
        int client_sock = accept(sock, NULL, 0);

        unsigned short count = 0;
        for(Service& service : services)
        {
            if(service.is_running) count++;
        }

        bool is_ok = (count == services.size());
        send(client_sock, &is_ok, sizeof(is_ok), 0);
        close(client_sock);
    }
}

void interrupt(int signal)
{
    healthcheck_run = false;
    close(sock);
    exit(0);
}

void register_signal(int sig)
{
    if(SIG_ERR == signal(sig, interrupt)) 
    {
        std::cerr << "Cannot register signal !" << std::endl;
        exit(2);
    }
}

int main(int argc, char const *argv[])
{
    if(argc < 2) {
        std::cerr << "Syntax : " << argv[0] << " <config_dir>" << std::endl;
        return 1;
    }

    register_signal(SIGINT);
    register_signal(SIGTERM);

    auto services = loadConfig(argv[1]);    
    std::thread healthcheck_th(healthcheck_socket, std::ref(services));

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

    healthcheck_run = false;
    healthcheck_th.join();

    return 0;
}
