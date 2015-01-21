file(GLOB headers
  ${GAMEKEEPER_PRIVATE_INCLUDE_DIR}/gamekeeper/model/genericgame.h
)

file(GLOB sources
  genericgame.cpp
)

add_library(gamekeeperModel SHARED ${sources} ${headers})

if(BUILD_USE_COTIRE)
  cotire(gamekeeperModel)
endif()
