include_directories(
  ${GAMEKEEPER_PRIVATE_INCLUDE_DIR}
  ${GAMEKEEPER_INCLUDE_DIR}
  ${GAMEKEEPER_LIBTEST_INCLUDE_DIR}
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

file(GLOB sources
  curlfiledownloader.cpp
  filedownloader.cpp
  xdgpaths.cpp
  ${platform_sources}
)

add_executable(gamekeeperUnitTestsCore ${sources})
target_link_libraries(gamekeeperUnitTestsCore
  gamekeeperUnitTest
  gamekeeperCore
  ${GTEST_BOTH_LIBRARIES}
)

if(MINGW)
  add_definitions(-DBOOST_THREAD_USE_LIB)
  target_link_libraries(gamekeeperUnitTestsCore ${Boost_THREAD_LIBRARY})
endif()

if(BUILD_USE_COTIRE)
  cotire(gamekeeperUnitTestsCore)
endif()

add_test(gamekeeperUnitTestsCore gamekeeperUnitTestsCore${CMAKE_EXECUTABLE_SUFFIX})
