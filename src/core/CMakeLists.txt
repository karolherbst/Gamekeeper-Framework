include_directories(
  ${GAMELIB_PRIVATE_INCLUDE_DIR}/gamelib/core
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${Boost_INCLUDE_DIRS}
  ${CURL_INCLUDE_DIRS}
  ${HYPODERMIC_INCLUDE_DIRS}
  ${LOG4CPP_INCLUDE_DIR}
)

if(LINUX)
  file(GLOB linux_headers
    ${GAMELIB_PRIVATE_INCLUDE_DIR}/gamelib/core/linuxinformation.h
  )

  file(GLOB linux_sources
    linuxinformation.cpp
  )
endif()

file(GLOB headers
  pch.h
  ${GAMELIB_PRIVATE_INCLUDE_DIR}/gamelib/core/curlFileDownloader.h
  ${GAMELIB_PRIVATE_INCLUDE_DIR}/gamelib/core/curlHelper.h
  ${GAMELIB_PRIVATE_INCLUDE_DIR}/gamelib/core/log4cppLogger.h
  ${GAMELIB_PRIVATE_INCLUDE_DIR}/gamelib/core/log4cpploggerFactory.h
  ${GAMELIB_PRIVATE_INCLUDE_DIR}/gamelib/core/log4cppLoggerStream.h
  ${GAMELIB_PRIVATE_INCLUDE_DIR}/gamelib/core/xdgpaths.h
  ${linux_headers}
)

file(GLOB sources
  curlFileDownloader.cpp
  curlHelper.cpp
  log4cppLogger.cpp
  log4cpploggerFactory.cpp
  log4cppLoggerStream.cpp
  xdgpaths.cpp
  ${linux_sources}
)

add_library(gamelibCore SHARED ${sources} ${headers})
target_link_libraries(gamelibCore
  ${Boost_FILESYSTEM_LIBRARY}
  ${CURL_LIBRARIES}
  ${LOG4CPP_LIBRARIES}
  ${Boost_SYSTEM_LIBRARY}
)
