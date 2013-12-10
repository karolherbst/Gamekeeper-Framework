include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
)

file(GLOB headers
  pch.h
)

file(GLOB sources
  main.cpp
)

add_library(gamelibClient SHARED ${sources} ${headers})
install_library(gamelibClient)
