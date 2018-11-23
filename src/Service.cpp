#include "Service.hpp"

Service::Service(ServiceConfig _config, std::shared_ptr<Logger> _logger)
: config(_config), logger(_logger)
{
}

Service::~Service()
{
    if(thread)
    {
        thread->join();
        delete thread;
    }    
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

void Service::thread_func()
{
    unsigned short retry = 0;
    flags |= ServiceFlag::KEEP_RUNNING;
    
    do
    {
        logger->log(config.name, "Starting...");
        auto stream = popen((config.command + " 2>&1").c_str(), "r");

        if(stream)
        {
            flags |= ServiceFlag::RUNNING;

            while(!feof(stream))
            {
                logger->log(config.name, read_line(stream));
            }

            pclose(stream);

            if(config.restart)
            {
                flags &= ~ServiceFlag::RUNNING;
                retry++;
                if(retry >= config.max_retry) flags &= ~ServiceFlag::KEEP_RUNNING;
            }
            flags |= ServiceFlag::RUNNED_ONCE;
        }   
    }
    while((flags & ServiceFlag::KEEP_RUNNING) && config.restart);

    if(config.restart)
    {
        // We notify that we can't run the service if the service is meant to restart
        logger->log(config.name, "Service didn't start after ", config.max_retry, " retries.");
    }    
    else
    {
        if(flags & ServiceFlag::RUNNED_ONCE)
        {
            logger->log(config.name, "Service executed once as expected.");
        }
        else
        {
            logger->log(config.name, "Service didn't executed once.");
        }
    }
}

void Service::launch()
{
    thread = new std::thread(&Service::thread_func, this);
}

bool Service::healthcheck()
{
    if(config.restart)
    {
        // When the service restarts every times it stops
        return ((flags & ServiceFlag::RUNNING) == ServiceFlag::RUNNING);
    }
    else
    {
        // If the service needs just to run once
        return ((flags & ServiceFlag::RUNNED_ONCE) == ServiceFlag::RUNNED_ONCE);
    }
}
