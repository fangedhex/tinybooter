file(GLOB_RECURSE headers CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h")
file(GLOB_RECURSE sources CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

include(${LIBS_DIR}/cli11.cmake)
include(${LIBS_DIR}/spdlog.cmake)
include(${LIBS_DIR}/glob.cmake)
include(${LIBS_DIR}/yaml-cpp.cmake)
include(${LIBS_DIR}/reproc.cmake)
include(${LIBS_DIR}/httplib.cmake)

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
