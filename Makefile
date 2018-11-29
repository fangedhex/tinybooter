CC=clang
CCX=clang++

SRC=src/Kernel.cpp src/ConfigLoader.cpp src/Logger.cpp src/main.cpp src/Service.cpp src/CliService.cpp

all:
	mkdir -p objects
	$(CC) -c -Iinclude/ lib/ini.c -o objects/ini.o
	$(CCX) --std=c++17 -lpthread -O2 -s -Iinclude/ -Ilib/ objects/ini.o lib/INIReader.cpp $(SRC) -o tinybooter
	$(CCX) --std=c++17 -O2 -s -Iinclude/ src/healthcheck.cpp -o healthcheck
