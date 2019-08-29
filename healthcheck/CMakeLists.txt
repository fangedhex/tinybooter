file(GLOB_RECURSE SOURCE_FILES src/*.cpp)

add_executable(healthcheck ${SOURCE_FILES})
target_link_libraries(healthcheck PRIVATE core)
