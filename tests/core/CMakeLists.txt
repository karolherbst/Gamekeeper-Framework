include_directories(
  ${GTEST_INCLUDE_DIRS}
)

file(GLOB sources
  linuxinformation.cpp
)

add_executable(gamelibUnitTests ${sources} ${headers})
target_link_libraries(gamelibUnitTests
  ${GTEST_BOTH_LIBRARIES}
)

add_test(gamelibUnitTests gamelibUnitTests)
