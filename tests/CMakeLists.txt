if(BUILD_TESTS)
  add_subdirectory(core)
endif()

if(BUILD_PROTOTYPES)
  add_subdirectory(prototyping)
endif()

# some important dependencies
if(BUILD_TESTS)
  if(NOT GTEST_FOUND)
    add_dependencies(gamelibUnitTests gtest)
  endif()

  if(NOT HYPODERMIC_FOUND)
    add_dependencies(gamelibUnitTests hypodermic)
  endif()
endif()
