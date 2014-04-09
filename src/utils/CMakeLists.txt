include_directories(
  ${GAMEKEEPER_PRIVATE_INCLUDE_DIR}/gamekeeper/utils
  ${CMAKE_CURRENT_SOURCE_DIR}
)

file(GLOB headers
  pch.h
)

add_custom_target(gamekeeperUtils ${sources} ${headers})
