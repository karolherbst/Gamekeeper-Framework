# include gloabel directory always

add_custom_target(headers
  SOURCES
    gamelib/client/gamelib.h

    gamelib/core/common.h
    gamelib/core/filedownloader.h
    gamelib/core/interface.h

    gamelib/log/logger.h
)
