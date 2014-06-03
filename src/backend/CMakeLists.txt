include_directories(
  ${GAMEKEEPER_PRIVATE_INCLUDE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${Boost_INCLUDE_DIRS}
  ${JANSSON_INCLUDE_DIR}
  ${JANSSON_PATH_INCLUDE_DIR}
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

file(GLOB headers
  pch.h
  ${GAMEKEEPER_PRIVATE_INCLUDE_DIR}/gamekeeper/backend/httppostloginhandler.h
  ${GAMEKEEPER_PRIVATE_INCLUDE_DIR}/gamekeeper/backend/jsongamelistparser.h
  ${GAMEKEEPER_PRIVATE_INCLUDE_DIR}/gamekeeper/backend/storeconfiguration.h
  ${GAMEKEEPER_PRIVATE_INCLUDE_DIR}/gamekeeper/backend/storeconfigurator.h
  ${GAMEKEEPER_PRIVATE_INCLUDE_DIR}/gamekeeper/backend/storemanager.h
  ${GAMEKEEPER_PRIVATE_INCLUDE_DIR}/gamekeeper/backend/xmlgamelistparser.h
  ${platform_headers}
)

file(GLOB sources
  httppostloginhandler.cpp
  jsongamelistparser.cpp
  storeconfigurator.cpp
  storeconfiguration.cpp
  storemanager.cpp
  xmlgamelistparser.cpp
  ${platform_sources}
)

add_library(gamekeeperBackend SHARED ${sources} ${headers})
target_link_libraries(gamekeeperBackend
  ${JANSSON_LIBRARIES}
  ${PUGIXML_LIBRARIES}
  jansson_path
)

if(MINGW)
  target_link_libraries(gamekeeperBackend
  )
endif()
