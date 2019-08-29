file(GLOB_RECURSE INCLUDE_FILES include/*.h)
add_library(core STATIC ${INCLUDE_FILES})
target_include_directories(core PUBLIC include)
set_target_properties(core PROPERTIES LINKER_LANGUAGE CXX)
