include(FetchContent)

set(REPROC++ ON)
FetchContent_Declare(
  reproc
  GIT_REPOSITORY  https://github.com/DaanDeMeyer/reproc.git
  GIT_TAG         v14.2.2
)
FetchContent_MakeAvailable(reproc)
