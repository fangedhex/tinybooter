cmake_minimum_required(VERSION 3.14 FATAL_ERROR)
project(
  tinybooter
  VERSION 1.0.0
)

include(cmake/CPM.cmake)

# System to avoid building inside our source directory
if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
  message(
    FATAL_ERROR
      "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there."
  )
endif()
set(LIBS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/libs")

add_subdirectory(core)
add_subdirectory(main)
add_subdirectory(test)
