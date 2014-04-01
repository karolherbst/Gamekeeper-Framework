include_directories(
  ${GAMEKEEPER_PRIVATE_INCLUDE_DIR}/gamekeeper/core
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${Boost_INCLUDE_DIRS}
  ${CURL_INCLUDE_DIRS}
  ${HYPODERMIC_INCLUDE_DIRS}
  ${LOG4CPP_INCLUDE_DIR}
)

if(WIN32)
  file(GLOB platform_headers
    ${GAMEKEEPER_PRIVATE_INCLUDE_DIR}/gamekeeper/core/windowsinformation.h
  )

  file(GLOB platform_sources
    windowsinformation.cpp
  )
elseif(LINUX)
  file(GLOB platform_headers
    ${GAMEKEEPER_PRIVATE_INCLUDE_DIR}/gamekeeper/core/linuxinformation.h
  )

  file(GLOB platform_sources
    linuxinformation.cpp
  )
endif()

file(GLOB headers
  pch.h
  ${GAMEKEEPER_PRIVATE_INCLUDE_DIR}/gamekeeper/core/boostpopropertyresolver.h
  ${GAMEKEEPER_PRIVATE_INCLUDE_DIR}/gamekeeper/core/curlfiledownloader.h
  ${GAMEKEEPER_PRIVATE_INCLUDE_DIR}/gamekeeper/core/curlhelper.h
  ${GAMEKEEPER_PRIVATE_INCLUDE_DIR}/gamekeeper/core/log4cppLogger.h
  ${GAMEKEEPER_PRIVATE_INCLUDE_DIR}/gamekeeper/core/log4cpploggerFactory.h
  ${GAMEKEEPER_PRIVATE_INCLUDE_DIR}/gamekeeper/core/log4cppLoggerStream.h
  ${GAMEKEEPER_PRIVATE_INCLUDE_DIR}/gamekeeper/core/xdgpaths.h
  ${platform_headers}
)

file(GLOB sources
  boostpopropertyresolver.cpp
  curlfiledownloader.cpp
  curlhelper.cpp
  log4cppLogger.cpp
  log4cpploggerFactory.cpp
  log4cppLoggerStream.cpp
  xdgpaths.cpp
  ${platform_sources}
)

add_library(gamekeeperCore SHARED ${sources} ${headers})
target_link_libraries(gamekeeperCore
  ${Boost_FILESYSTEM_LIBRARY}
  ${CURL_LIBRARIES}
  ${LOG4CPP_LIBRARIES}
  ${Boost_SYSTEM_LIBRARY}
)
