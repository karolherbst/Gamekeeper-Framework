# include gloabel directory always

add_custom_target(private_headers
  SOURCES
    gamekeeper/backend/authmanager.h
    gamekeeper/backend/gamelistparser.h
    gamekeeper/backend/loginhandler.h

    gamekeeper/core/loggerFactory.h
    gamekeeper/core/nativethreadhelper.h
)
