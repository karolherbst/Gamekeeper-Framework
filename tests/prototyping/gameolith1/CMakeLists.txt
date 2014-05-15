include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${GAMEKEEPER_INCLUDE_DIR}
  ${Boost_INCLUDE_DIRS}
  ${HYPODERMIC_INCLUDE_DIRS}
  ${JSONCPP_INCLUDE_DIRS}
)

file(GLOB headers
  gameolithprototype.h
)

file(GLOB sources
  gameolithprototype.cpp
)

add_executable(GameolithPrototype ${sources} ${headers})
target_link_libraries(GameolithPrototype
  gamekeeperClientMain
  ${JSONCPP_LIBRARIES}
)
