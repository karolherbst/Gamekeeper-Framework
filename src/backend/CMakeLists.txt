include_directories(
  ${GAMEKEEPER_PRIVATE_INCLUDE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${Boost_INCLUDE_DIRS}
  ${JANSSON_INCLUDE_DIR}
  ${JANSSON_PATH_INCLUDE_DIR}
  ${LIBSECRET_INCLUDE_DIRS}
  ${PUGIXML_INCLUDE_DIRS}
)

if(WIN32)
  file(GLOB platform_headers
  )

  file(GLOB platform_sources
  )
elseif(LINUX)
  file(GLOB platform_headers
  )

  file(GLOB platform_sources
  )

endif()

if(WITH_LIBSECRET)
  file(GLOB libsecret_manager
    libsecretmanager.cpp
  )
  add_definitions(-DSECRET_WITH_UNSTABLE -DSECRET_API_SUBJECT_TO_CHANGE)
endif()

file(GLOB headers
  ${GAMEKEEPER_PRIVATE_INCLUDE_DIR}/gamekeeper/backend/httppostloginhandler.h
  ${GAMEKEEPER_PRIVATE_INCLUDE_DIR}/gamekeeper/backend/jsongamelistparser.h
  ${GAMEKEEPER_PRIVATE_INCLUDE_DIR}/gamekeeper/backend/libsecretmanager.h
  ${GAMEKEEPER_PRIVATE_INCLUDE_DIR}/gamekeeper/backend/storeconfiguration.h
  ${GAMEKEEPER_PRIVATE_INCLUDE_DIR}/gamekeeper/backend/storeconfigurator.h
  ${GAMEKEEPER_PRIVATE_INCLUDE_DIR}/gamekeeper/backend/storemanager.h
  ${GAMEKEEPER_PRIVATE_INCLUDE_DIR}/gamekeeper/backend/xmlgamelistparser.h
  ${platform_headers}
)

file(GLOB sources
  authmanager.cpp
  httppostloginhandler.cpp
  jsongamelistparser.cpp
  storeconfigurator.cpp
  storeconfiguration.cpp
  storemanager.cpp
  xmlgamelistparser.cpp
  ${platform_sources}
)

add_library(gamekeeperBackend SHARED ${sources} ${headers} ${libsecret_manager})
target_link_libraries(gamekeeperBackend
  gamekeeperCore
  ${JANSSON_LIBRARIES}
  ${LIBSECRET_LIBRARIES}
  ${PUGIXML_LIBRARIES}
  jansson_path
)

if(BUILD_USE_COTIRE)
  cotire(gamekeeperBackend)
endif()

if(MINGW)
  target_link_libraries(gamekeeperBackend
  )
endif()
