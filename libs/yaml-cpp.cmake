include(FetchContent)

set(YAML_CPP_BUILD_TESTS OFF)

FetchContent_Declare(
  yaml-cpp
  GIT_REPOSITORY  https://github.com/jbeder/yaml-cpp.git
  GIT_TAG         yaml-cpp-0.6.3
)
FetchContent_MakeAvailable(yaml-cpp)
