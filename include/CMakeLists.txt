# include gloabel directory always

add_custom_target(headers
  SOURCES
    std_compat/mutex
    std_compat/thread

    gamekeeper/client/autowire.h
    gamekeeper/client/game.h
    gamekeeper/client/gamekeeper.h
    gamekeeper/client/hypodermic.h
    gamekeeper/client/store.h
    gamekeeper/client/storecontroller.h

    gamekeeper/core/bundlepaths.h
    gamekeeper/core/common.h
    gamekeeper/core/filedownloader.h
    gamekeeper/core/filedownloaderfactory.h
    gamekeeper/core/interface.h
    gamekeeper/core/logger.h
    gamekeeper/core/loggerStream.h
    gamekeeper/core/osinformation.h
    gamekeeper/core/propertyresolver.h
    gamekeeper/core/threadfactory.h
    gamekeeper/core/userpaths.h

    gamekeeper/model/game.h
    gamekeeper/model/store.h

    gamekeeper/utils/containerutils.h
    gamekeeper/utils/stringutils.h
)

install(DIRECTORY gamekeeper
  DESTINATION ${CMAKE_INSTALL_FULL_INCLUDEDIR}
)
