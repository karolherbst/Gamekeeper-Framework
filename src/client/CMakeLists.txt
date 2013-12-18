include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${HYPODERMIC_LIBRARIES}
)

file(GLOB headers
  pch.h
)

file(GLOB sources
  main.cpp
)

add_library(gamelibClient SHARED ${sources} ${headers})
target_link_libraries(gamelibClient
  gamelibCore
  ${HYPODERMIC_LIBRARIES}
  ${Boost_SYSTEM_LIBRARY}
)

install_library(gamelibClient)
