# include gloabel directory always

add_custom_target(private_headers
  SOURCES
    gamekeeper/backend/gamelistparser.h
    gamekeeper/backend/loginhandler.h

    gamekeeper/backend/security/authmanager.h
    gamekeeper/backend/security/token.h

    gamekeeper/core/loggerFactory.h
    gamekeeper/core/nativethreadhelper.h
)
