# include gloabel directory always
include_directories(
  ${GAMEKEEPER_PRIVATE_INCLUDE_DIR}
  ${GAMEKEEPER_INCLUDE_DIR}
)

# needed for windows
add_definitions(-DBUILDING_DLL)

add_subdirectory(utils)
add_subdirectory(client)
add_subdirectory(core)
add_subdirectory(bindings)

# some important dependencies
if(NOT CURL_FOUND)
  add_dependencies(gamekeeperCore curl)
endif()
if(NOT HYPODERMIC_FOUND)
  add_dependencies(gamekeeperClient hypodermic)
endif()
if(NOT LOG4CPP_FOUND)
  add_dependencies(gamekeeperClient log4cpp)
endif()
