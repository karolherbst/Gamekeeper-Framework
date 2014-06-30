include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${Boost_INCLUDE_DIRS}
  ${HYPODERMIC_INCLUDE_DIRS}
)

file(GLOB headers
  ${GAMEKEEPER_PRIVATE_INCLUDE_DIR}/gamekeeper/client/gameimpl.h
  ${GAMEKEEPER_PRIVATE_INCLUDE_DIR}/gamekeeper/client/storeimpl.h
  ${GAMEKEEPER_PRIVATE_INCLUDE_DIR}/gamekeeper/client/storecontrollerimpl.h
  runtime.h
)

file(GLOB sources
  gameimpl.cpp
  runtime.cpp
  storeimpl.cpp
  storecontrollerimpl.cpp
)

file(GLOB main_sources
  main.cpp
)

add_library(gamekeeperClient SHARED ${sources} ${headers})
target_link_libraries(gamekeeperClient
  gamekeeperBackend
  gamekeeperCore
  ${HYPODERMIC_LIBRARIES}
  ${Boost_PROGRAM_OPTIONS_LIBRARY}
  ${Boost_SYSTEM_LIBRARY}
)

if(BUILD_USE_COTIRE)
  cotire(gamekeeperClient)
endif()

install_library(gamekeeperClient)

add_library(gamekeeperClientMain STATIC ${main_sources})
target_link_libraries(gamekeeperClientMain
  gamekeeperClient
  ${CMAKE_DL_LIBS}
)

if(BUILD_USE_COTIRE)
  cotire(gamekeeperClientMain)
endif()

#install_library(gamekeeperClientMain)
