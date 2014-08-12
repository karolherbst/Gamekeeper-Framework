# include gloabel directory always
include_directories(
  ${GAMEKEEPER_PRIVATE_INCLUDE_DIR}
  ${GAMEKEEPER_INCLUDE_DIR}
)

# needed for windows
add_definitions(-DBUILDING_DLL)

if(WITH_LIBSECRET)
  add_definitions(-DWITH_LIBSECRET)
endif()

add_subdirectory(utils)
add_subdirectory(model)
add_subdirectory(client)
add_subdirectory(core)
add_subdirectory(bindings)
add_subdirectory(backend)

# some important dependencies
if(NOT CURL_FOUND)
  add_dependencies(gamekeeperCore curl)
endif()
if(NOT HYPODERMIC_FOUND)
  add_dependencies(gamekeeperClient hypodermic)
endif()
if(NOT JANSSON_FOUND)
  add_dependencies(gamekeeperBackend jansson)
endif()
if(NOT LOG4CPP_FOUND)
  add_dependencies(gamekeeperClient log4cpp)
endif()
