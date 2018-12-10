#include "Kernel.hpp"

#include "ConfigLoader.hpp"

Kernel::Kernel()
: cliService(services, logger)
{
}

int Kernel::execute(int argc, const char** argv)
{
    {
        // We load our configurations into services
        ConfigLoader loader;
        for(auto config : loader.getServicesConfigFromIni(argv[1]))
        {
            services.emplace_back(config, logger);
        }
    }    

    // Waiting for them to stop
    for(auto& service : services)
    {
        service.launch();
    }

    cliService.launch();

    return EXIT_SUCCESS;
}