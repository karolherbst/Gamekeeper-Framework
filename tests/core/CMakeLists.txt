include_directories(
  ${GAMEKEEPER_PRIVATE_INCLUDE_DIR}
  ${GAMEKEEPER_INCLUDE_DIR}
  ${Boost_INCLUDE_DIRS}
  ${GTEST_INCLUDE_DIRS}
  ${HYPODERMIC_INCLUDE_DIRS}
  ${MONGOOSE_INCLUDE_DIR}
)

add_definitions(-DTESTPATH=\"${CMAKE_CURRENT_BINARY_DIR}/test_runtime\")
add_definitions(-DWEBSERVERPATH=\"${CMAKE_CURRENT_SOURCE_DIR}/webserverfiles\")

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
  webserverfixture.h
)

file(GLOB sources
  curlfiledownloader.cpp
  defaultfixture.cpp
  stringutils.cpp
  webserverfixture.cpp
  xdgpaths.cpp
  ${platform_sources}
)

add_executable(gamekeeperUnitTests ${sources} ${headers})
target_link_libraries(gamekeeperUnitTests
  gamekeeperCore
  mongoose
  ${Boost_SYSTEM_LIBRARY}
  ${GTEST_BOTH_LIBRARIES}
  ${HYPODERMIC_LIBRARIES}
)

add_test(gamekeeperUnitTests gamekeeperUnitTests${CMAKE_EXECUTABLE_SUFFIX})
