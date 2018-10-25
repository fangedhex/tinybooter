CC=clang++

all:
	$(CC) --std=c++17 -g -Iinclude/ src/ini.c src/INIReader.cpp src/main.cpp -o tinybooter
