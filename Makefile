CC=clang
CXX=clang++

SRC=$(filter-out src/healthcheck.cpp, $(wildcard src/*.cpp))
TEST_SRC=$(filter-out src/main.cpp, $(SRC)) $(wildcard tests/*.cpp)

all:
	mkdir -p objects
	$(CC) -c -Iinclude/ lib/ini.c -o objects/ini.o
	$(CXX) --std=c++17 -lpthread -O2 -s -Iinclude/ -Ilib/ objects/ini.o lib/INIReader.cpp $(SRC) -o tinybooter
	$(CXX) --std=c++17 -O2 -s -Iinclude/ src/healthcheck.cpp -o healthcheck

test: all
	$(CXX) --std=c++17 -lpthread -lgtest -lgmock -O2 -s -Iinclude/ -Ilib/ objects/ini.o lib/INIReader.cpp $(TEST_SRC) -o testing
	./testing
