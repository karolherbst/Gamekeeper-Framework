include_directories(
  ${GAMEKEEPER_PRIVATE_INCLUDE_DIR}
  ${GAMEKEEPER_INCLUDE_DIR}
  ${Boost_INCLUDE_DIRS}
  ${GTEST_INCLUDE_DIRS}
  ${HYPODERMIC_INCLUDE_DIRS}
)

if(LINUX)
  file(GLOB platform_sources
    linuxinformation.cpp
  )
elseif(WIN32)
  file(GLOB platform_sources
    windowsinformation.cpp
  )
endif()

file(GLOB headers
  defaultfixture.h
)

file(GLOB sources
  defaultfixture.cpp
  xdgpaths.cpp
  ${platform_sources}
)

add_executable(gamekeeperUnitTests ${sources} ${headers})
target_link_libraries(gamekeeperUnitTests
  gamekeeperCore
  ${Boost_SYSTEM_LIBRARY}
  ${GTEST_BOTH_LIBRARIES}
  ${HYPODERMIC_LIBRARIES}
)

add_test(gamekeeperUnitTests gamekeeperUnitTests)
