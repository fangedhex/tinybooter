CC=clang++

FILES=src/ini.c src/INIReader.cpp src/main.cpp

all:
	$(CC) --std=c++17 -lpthread -O2 -s -Iinclude/ $(FILES) -o tinybooter
