include_directories(
  ${GAMEKEEPER_PRIVATE_INCLUDE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
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
  ${GAMEKEEPER_PRIVATE_INCLUDE_DIR}/gamekeeper/backend/xmlgamelistparser.h
  ${platform_headers}
)

file(GLOB sources
  xmlgamelistparser.cpp
  ${platform_sources}
)

add_library(gamekeeperBackend SHARED ${sources} ${headers})
target_link_libraries(gamekeeperBackend
)

if(MINGW)
  target_link_libraries(gamekeeperBackend
  )
endif()
