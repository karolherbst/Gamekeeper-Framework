include_directories(
  ${GAMEKEEPER_PRIVATE_INCLUDE_DIR}/gamekeeper/utils
  ${CMAKE_CURRENT_SOURCE_DIR}
)

file(GLOB headers
)

add_custom_target(gamekeeperUtils ${sources} ${headers})
