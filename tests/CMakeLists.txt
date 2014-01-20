if(BUILD_TESTS)
  add_subdirectory(core)
endif()

if(BUILD_PROTOTYPES)
  add_subdirectory(prototyping)
endif()

# some important dependencies
if(NOT GTEST_FOUND)
  if(BUILD_TESTS)
    add_dependencies(gamelibUnitTests gtest)
  endif()
endif()
