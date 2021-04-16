cmake_minimum_required(VERSION 3.1.2)
project(tinybooter)

execute_process(COMMAND conan install -if build --build=missing .
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)

add_executable(${PROJECT_NAME} src/main.cpp)
target_link_libraries(${PROJECT_NAME} CONAN_PKG::spdlog CONAN_PKG::fmt)
