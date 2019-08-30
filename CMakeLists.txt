cmake_minimum_required(VERSION 3.10)
project(tinybooter)

#list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake )

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(libs/googletest)

add_subdirectory(core)
add_subdirectory(main)
add_subdirectory(healthcheck)
