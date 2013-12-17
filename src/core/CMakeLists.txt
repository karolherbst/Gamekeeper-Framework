include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
)

file(GLOB headers
  pch.h
  curlFileDownloader.h
)

file(GLOB sources
  curlFileDownloader.cpp
)

add_library(gamelibCore STATIC ${sources} ${headers})
