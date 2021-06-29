file(GLOB_RECURSE headers CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h")
file(GLOB_RECURSE sources CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

CPMAddPackage("gh:CLIUtils/CLI11@1.9.1")
CPMAddPackage("gh:gabime/spdlog@1.8.5")
CPMAddPackage("gh:p-ranav/glob@1.0.0#master")
CPMAddPackage("gh:jbeder/yaml-cpp@0.6.3#yaml-cpp-0.6.3")
set(REPROC++ ON)
CPMAddPackage("gh:DaanDeMeyer/reproc@14.2.2")
CPMAddPackage("gh:yhirose/cpp-httplib@0.9.0")

add_library(
  core
  ${headers}
  ${sources}
)

#set_target_properties(core PROPERTIES ENABLE_EXPORTS 1)
set_target_properties(core PROPERTIES
  CXX_STANDARD 17
  CXX_STANDARD_REQUIRED ON
  CXX_EXTENSIONS OFF
)

target_include_directories(core PUBLIC include)
target_link_libraries(
  core
  CLI11
  spdlog
  Glob
  yaml-cpp
  reproc++
  httplib
)
