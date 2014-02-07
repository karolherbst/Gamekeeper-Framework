include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${Boost_INCLUDE_DIRS}
  ${HYPODERMIC_INCLUDE_DIRS}
)

file(GLOB headers
  pch.h
  runtime.h
)

file(GLOB sources
  runtime.cpp
)

file(GLOB main_sources
  main.cpp
)

add_library(gamelibClient SHARED ${sources} ${headers})
target_link_libraries(gamelibClient
  gamelibCore
  ${HYPODERMIC_LIBRARIES}
  ${Boost_SYSTEM_LIBRARY}
)
install_library(gamelibClient)

add_library(gamelibClientMain STATIC ${main_sources})
target_link_libraries(gamelibClientMain
  gamelibClient
)
#install_library(gamelibClientMain)
