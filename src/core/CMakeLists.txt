include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${HYPODERMIC_INCLUDE_DIRS}
  ${LOG4CPP_INCLUDE_DIR}
)

if(LINUX)
  file(GLOB linux_headers
    linuxinformation.h
    linuxpaths.h
  )

  file(GLOB linux_sources
    linuxinformation.cpp
    linuxpaths.cpp
  )
endif()

file(GLOB headers
  pch.h
  curlFileDownloader.h
  curlHelper.h
  log4cppLogger.h
  log4cpploggerFactory.h
  log4cppLoggerStream.h
  ${linux_headers}
)

file(GLOB sources
  curlFileDownloader.cpp
  curlHelper.cpp
  log4cppLogger.cpp
  log4cpploggerFactory.cpp
  log4cppLoggerStream.cpp
  ${linux_sources}
)

add_library(gamelibCore STATIC ${sources} ${headers})
target_link_libraries(gamelibCore
  ${CURL_LIBRARIES}
  ${LOG4CPP_LIBRARIES}
)
