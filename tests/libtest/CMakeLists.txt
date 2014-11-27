include_directories(
  ${GAMEKEEPER_PRIVATE_INCLUDE_DIR}
  ${GAMEKEEPER_INCLUDE_DIR}
  ${HYPODERMIC_INCLUDE_DIRS}
  ${MONGOOSE_INCLUDE_DIR}
)

set(GAMEKEEPER_LIBTEST_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR} PARENT_SCOPE)
add_definitions(-DTESTPATH=\"${CMAKE_CURRENT_BINARY_DIR}/test_runtime\")
add_definitions(-DWEBSERVERPATH=\"${CMAKE_CURRENT_SOURCE_DIR}/webserverfiles\")

file(GLOB headers
  defaultfixture.h
  webserverfixture.h
)

file(GLOB sources
  defaultfixture.cpp
  webserverfixture.cpp
)

add_library(gamekeeperUnitTest STATIC ${sources} ${headers})
target_link_libraries(gamekeeperUnitTest
  mongoose
  ${HYPODERMIC_LIBRARIES}
)

if(BUILD_USE_COTIRE)
  cotire(gamekeeperUnitTest)
endif()
