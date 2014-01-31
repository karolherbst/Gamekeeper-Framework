include_directories(
  ${GAMELIB_PRIVATE_INCLUDE_DIR}
  ${GAMELIB_INCLUDE_DIR}
  ${Boost_INCLUDE_DIRS}
  ${GTEST_INCLUDE_DIRS}
  ${HYPODERMIC_INCLUDE_DIRS}
)

if(LINUX)
  file(GLOB linux_sources
    linuxinformation.cpp
  )
endif()

file(GLOB headers
  defaultfixture.h
)

file(GLOB sources
  defaultfixture.cpp
  ${linux_sources}
)

add_executable(gamelibUnitTests ${sources} ${headers})
target_link_libraries(gamelibUnitTests
  gamelibCore
  ${Boost_SYSTEM_LIBRARY}
  ${GTEST_BOTH_LIBRARIES}
  ${HYPODERMIC_LIBRARIES}
)

add_test(gamelibUnitTests gamelibUnitTests)