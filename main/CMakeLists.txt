file(GLOB_RECURSE SOURCE_FILES src/*.cpp src/*.c)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_include_directories(${PROJECT_NAME} PRIVATE include)

# Core lib
target_link_libraries(${PROJECT_NAME} core)

# Thread lib
find_package (Threads REQUIRED)
target_link_libraries(${PROJECT_NAME} Threads::Threads)

# Tests
file(GLOB_RECURSE TEST_SOURCES_FILE src/*.cpp src/*.c tests/*.cpp)
list(FILTER TEST_SOURCES_FILE EXCLUDE REGEX src/main.cpp)
add_executable(${PROJECT_NAME}_test ${TEST_SOURCES_FILE})
target_include_directories(${PROJECT_NAME}_test PRIVATE include)
target_include_directories(${PROJECT_NAME}_test PRIVATE tests)
target_link_libraries(${PROJECT_NAME}_test core Threads::Threads gtest gmock)
