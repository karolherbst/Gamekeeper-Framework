include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
)

file(GLOB headers
  pch.h
  gamelib.h
)

file(GLOB sources
  gamelib.cpp
)

add_library(gamelibClient SHARED ${sources} ${headers})
install_library(gamelibClient)
