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
# CONAN SETUP
### ### ### ### ###

# Download automatically, you can also just copy the conan.cmake file
if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
   message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
   file(DOWNLOAD "https://raw.githubusercontent.com/conan-io/cmake-conan/master/conan.cmake"
                  "${CMAKE_BINARY_DIR}/conan.cmake")
endif()

include(${CMAKE_BINARY_DIR}/conan.cmake)

conan_cmake_run(
  REQUIRES
  libyaml/0.2.5
  yaml-cpp/0.6.3
  spdlog/[>=1.4.1]
  cli11/1.9.1
  GENERATORS cmake
  BASIC_SETUP CMAKE_TARGETS
  BUILD missing
  PROFILE ${CMAKE_SOURCE_DIR}/.conan/profile.ini
)

### ### ### ### ###
# LIBS WITH FETCHCONTENT
### ### ### ### ###
include(FetchContent)

set(REPROC++ ON)
FetchContent_Declare(
  reproc
  GIT_REPOSITORY  https://github.com/DaanDeMeyer/reproc.git
  GIT_TAG         v14.2.2
)
FetchContent_MakeAvailable(reproc)

### ### ### ### ###
# PROJECT SETUP
### ### ### ### ###

file(GLOB_RECURSE headers "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h")
file(GLOB_RECURSE sources "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

add_executable(${PROJECT_NAME} ${headers} ${sources})
set_target_properties(${PROJECT_NAME} PROPERTIES
  CXX_STANDARD 17
  CXX_STANDARD_REQUIRED ON
  CXX_EXTENSIONS OFF
)
target_include_directories(${PROJECT_NAME} PRIVATE "include")
target_link_libraries(
  ${PROJECT_NAME}
  CONAN_PKG::cli11
  CONAN_PKG::spdlog
  CONAN_PKG::yaml-cpp
  reproc++
)
