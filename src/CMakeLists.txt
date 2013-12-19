# include gloabel directory always
include_directories(
  ${GAMELIB_INCLUDE_DIR}
)

add_subdirectory(client)
add_subdirectory(core)
add_subdirectory(frontend)
add_subdirectory(model)
