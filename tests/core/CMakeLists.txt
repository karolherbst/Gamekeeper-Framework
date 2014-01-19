include_directories(
  ${GAMELIB_PRIVATE_INCLUDE_DIR}
  ${GAMELIB_INCLUDE_DIR}
  ${Boost_INCLUDE_DIRS}
  ${GTEST_INCLUDE_DIRS}
)

file(GLOB sources
  linuxinformation.cpp
)

add_executable(gamelibUnitTests ${sources} ${headers})
target_link_libraries(gamelibUnitTests
  gamelibCore
  ${Boost_SYSTEM_LIBRARY}
  ${GTEST_BOTH_LIBRARIES}
)

add_test(gamelibUnitTests gamelibUnitTests)
