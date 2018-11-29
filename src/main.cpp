#include <iostream>
#include <unistd.h>
#include <signal.h>

#include "Kernel.hpp"

void interrupt(int signal)
{
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

    Kernel kernel;
    return kernel.execute(argc, argv);
}
