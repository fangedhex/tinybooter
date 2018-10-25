CC=clang++

FILES=src/ini.c src/INIReader.cpp src/main.cpp

all:
	$(CC) --std=c++17 -lpthread -g -Iinclude/ $(FILES) -o tinybooter
