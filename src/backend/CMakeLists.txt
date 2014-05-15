include_directories(
  ${GAMEKEEPER_PRIVATE_INCLUDE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${Boost_INCLUDE_DIRS}
  ${JSONCPP_INCLUDE_DIRS}
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
  ${GAMEKEEPER_PRIVATE_INCLUDE_DIR}/gamekeeper/backend/jsongamelistparser.h
  ${GAMEKEEPER_PRIVATE_INCLUDE_DIR}/gamekeeper/backend/xmlgamelistparser.h
  ${platform_headers}
)

file(GLOB sources
  jsongamelistparser.cpp
  xmlgamelistparser.cpp
  ${platform_sources}
)

add_library(gamekeeperBackend SHARED ${sources} ${headers})
target_link_libraries(gamekeeperBackend
  ${JSONCPP_LIBRARIES}
)

if(MINGW)
  target_link_libraries(gamekeeperBackend
  )
endif()
