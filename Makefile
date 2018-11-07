CC=clang
CCX=clang++

all:
	$(CC) -c -Iinclude/ src/ini.c -o ini.o
	$(CCX) --std=c++17 -lpthread -O2 -s -Iinclude/ ini.o src/INIReader.cpp src/main.cpp -o tinybooter
	$(CCX) --std=c++17 -O2 -s -Iinclude/ src/healthcheck.cpp -o healthcheck
