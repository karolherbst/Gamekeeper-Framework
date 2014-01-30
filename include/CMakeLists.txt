# include gloabel directory always

add_custom_target(headers
  SOURCES
    gamelib/client/autowire.h
    gamelib/client/gamelib.h
    gamelib/client/hypodermic.h

    gamelib/core/common.h
    gamelib/core/filedownloader.h
    gamelib/core/httpfiledownloader.h
    gamelib/core/interface.h
    gamelib/core/logger.h
    gamelib/core/loggerStream.h
    gamelib/core/osinformation.h
    gamelib/core/ospaths.h

    gamelib/model/game.h
)
