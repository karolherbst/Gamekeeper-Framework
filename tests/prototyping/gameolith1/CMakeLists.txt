include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${GAMELIB_INCLUDE_DIR}
  ${HYPODERMIC_INCLUDE_DIRS}
)

file(GLOB headers
  gameolithprototype.h
)

file(GLOB sources
  gameolithprototype.cpp
)

add_executable(GameolithPrototype ${sources} ${headers})
target_link_libraries(GameolithPrototype
  gamelibClient
  gamelibModelBindings
)
