if(WIN32 AND NOT MINGW)
  message(FATAL_ERROR "windows build is currently only supported with mingw")
else()
  if(MINGW)
    set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
    find_package(Threads REQUIRED)
    set(LOG4CPP_INSTALL_DIR ${CMAKE_EXTERNAL_BINARY_DIR}/log4cpp)
    ExternalProject_Add(
      log4cpp
      URL ${LOG4CPP_URL}
      URL_MD5 ${LOG4CPP_MD5}
      UPDATE_COMMAND ""
      CONFIGURE_COMMAND ./configure
        --disable-dot
        --disable-doxygen
        --disable-html-docs
        --without-idsa
        --disable-latex-docs
        --enable-shared
        --disable-static
        --prefix=${LOG4CPP_INSTALL_DIR}
      BUILD_IN_SOURCE 1
    )
    set(LOG4CPP_LIBRARIES "${LOG4CPP_INSTALL_DIR}/lib/liblog4cpp.a")
    set(LOG4CPP_INCLUDE_DIR "${LOG4CPP_INSTALL_DIR}/include")
  else()
    message(FATAL_ERROR "building log4cpp isn't supported on plattform except mingw")
  endif()
endif()
