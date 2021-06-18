#file(GLOB_RECURSE test_headers CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/mocks/*.h")
file(GLOB_RECURSE test_sources CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

include(${LIBS_DIR}/googletest.cmake)

enable_testing()

add_executable(
  ${PROJECT_NAME}_test
  ${test_sources}
)
set_target_properties(${PROJECT_NAME}_test PROPERTIES
  CXX_STANDARD 17
  CXX_STANDARD_REQUIRED ON
  CXX_EXTENSIONS OFF
)

#target_include_directories(${PROJECT_NAME} PRIVATE "include")
target_link_libraries(
  ${PROJECT_NAME}_test
  ${PROJECT_NAME}
  gtest_main
)

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME}_test)
