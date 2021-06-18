include(FetchContent)

FetchContent_Declare(
  httplib
  GIT_REPOSITORY  https://github.com/yhirose/cpp-httplib.git
  GIT_TAG         v0.9.0
)
FetchContent_MakeAvailable(httplib)
