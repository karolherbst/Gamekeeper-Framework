include_directories(
  ${GAMEKEEPER_INCLUDE_DIR}
)

file(GLOB sources
  containerutils.cpp
  stringutils.cpp
)

add_executable(gamekeeperUnitTestsUtils ${sources})
target_link_libraries(gamekeeperUnitTestsUtils
  ${GTEST_BOTH_LIBRARIES}
)

if(BUILD_USE_COTIRE)
  cotire(gamekeeperUnitTestsUtils)
endif()

add_test(gamekeeperUnitTestsUtils gamekeeperUnitTestsUtils${CMAKE_EXECUTABLE_SUFFIX})
