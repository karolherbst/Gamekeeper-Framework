include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
)

file(GLOB headers
  pch.h
  curlFileDownloader.h
  curlHelper.h
)

file(GLOB sources
  curlFileDownloader.cpp
  curlHelper.cpp
)

add_library(gamelibCore STATIC ${sources} ${headers})
target_link_libraries(gamelibCore
  ${CURL_LIBRARIES}
)
