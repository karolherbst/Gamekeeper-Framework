# disable warnings and stuff, because test aren't about clean programming
add_compiler_flags(-w)

if(BUILD_TESTS)
  add_subdirectory(libtest)
  add_subdirectory(core)
endif()

if(BUILD_PROTOTYPES)
  add_subdirectory(prototyping)
endif()

# some important dependencies
if(BUILD_TESTS)
  if(NOT GTEST_FOUND)
    add_dependencies(gamekeeperUnitTestsCore gtest)
  endif()

  if(NOT HYPODERMIC_FOUND)
    add_dependencies(gamekeeperUnitTestsCore hypodermic)
  endif()
endif()
