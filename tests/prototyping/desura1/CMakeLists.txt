include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${GAMEKEEPER_PRIVATE_INCLUDE_DIR}
  ${GAMEKEEPER_INCLUDE_DIR}
  ${Boost_INCLUDE_DIRS}
  ${HYPODERMIC_INCLUDE_DIRS}
)

file(GLOB headers
  desuraprototype.h
)

file(GLOB sources
  desuraprototype.cpp
)

add_executable(DesuraPrototype ${sources} ${headers})
target_link_libraries(DesuraPrototype
  gamekeeperBackend
  gamekeeperClientMain
)
