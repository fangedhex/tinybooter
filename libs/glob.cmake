include(FetchContent)

FetchContent_Declare(
  Glob
  GIT_REPOSITORY  https://github.com/p-ranav/glob.git
  GIT_TAG         master
)
FetchContent_MakeAvailable(Glob)
