cmake_minimum_required(VERSION 3.1.2)
project(tinybooter)

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
  GENERATORS cmake
  BASIC_SETUP CMAKE_TARGETS
  BUILD missing
  PROFILE ${CMAKE_SOURCE_DIR}/.conan/profile.ini
)

### ### ### ### ###
# PROJECT SETUP
### ### ### ### ###

add_executable(${PROJECT_NAME} src/main.cpp include/Config.h)
set_target_properties(${PROJECT_NAME} PROPERTIES
  CXX_STANDARD 11
  CXX_STANDARD_REQUIRED ON
  CXX_EXTENSIONS OFF
)
target_include_directories(${PROJECT_NAME} PRIVATE include)
target_link_libraries(${PROJECT_NAME} CONAN_PKG::spdlog CONAN_PKG::yaml-cpp)
