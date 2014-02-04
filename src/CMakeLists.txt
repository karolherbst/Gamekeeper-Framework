# include gloabel directory always
include_directories(
  ${GAMELIB_PRIVATE_INCLUDE_DIR}
  ${GAMELIB_INCLUDE_DIR}
)

# needed for windows
add_definitions(-DBUILDING_DLL)

add_subdirectory(client)
add_subdirectory(core)
add_subdirectory(frontend)
add_subdirectory(model)
add_subdirectory(bindings)

# some important dependencies
if(NOT CURL_FOUND)
  add_dependencies(gamelibCore curl)
endif()
if(NOT HYPODERMIC_FOUND)
  add_dependencies(gamelibClient hypodermic)
endif()
if(NOT LOG4CPP_FOUND)
  add_dependencies(gamelibClient log4cpp)
endif()
