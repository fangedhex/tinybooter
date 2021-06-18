cmake_minimum_required(VERSION 3.14)
project(
  tinybooter
  VERSION 1.0.0
)

# System to avoid building inside our source directory
if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
  message(
    FATAL_ERROR
      "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there."
  )
endif()

### ### ### ### ###
# REQUIRED LIBRARIES SETUP
### ### ### ### ###

set(LIBS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/libs")
include(${LIBS_DIR}/cli11.cmake)
include(${LIBS_DIR}/spdlog.cmake)
include(${LIBS_DIR}/glob.cmake)
include(${LIBS_DIR}/yaml-cpp.cmake)
include(${LIBS_DIR}/reproc.cmake)
include(${LIBS_DIR}/httplib.cmake)

### ### ### ### ###
# PROJECT SETUP
### ### ### ### ###

file(GLOB_RECURSE headers CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h")
file(GLOB_RECURSE sources CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

add_executable(${PROJECT_NAME} ${headers} ${sources})
set_target_properties(${PROJECT_NAME} PROPERTIES
  CXX_STANDARD 17
  CXX_STANDARD_REQUIRED ON
  CXX_EXTENSIONS OFF
)
target_include_directories(${PROJECT_NAME} PRIVATE "include")
target_link_libraries(
  ${PROJECT_NAME}
  CLI11
  spdlog
  Glob
  yaml-cpp
  reproc++
  httplib
)
