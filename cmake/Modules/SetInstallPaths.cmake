if(UNIX)
  set(DEFAULT_INSTALL_DIR "/usr/local")
elseif(WIN32)
  set(DEFAULT_INSTALL_DIR "${CMAKE_SOURCE_DIR}/install")
endif()

set(CMAKE_INSTALL_PREFIX ${DEFAULT_INSTALL_DIR}
    CACHE STRING "${CMAKE_PROJECT_NAME} Install Prefix")

# on mingw someone might get the idea to install it in mingw system root
if(WIN32 AND NOT MINGW)
  # use GNUInstallDirs compatibility layer on win32
  set(CMAKE_INSTALL_FULL_BINDIR "${CMAKE_INSTALL_PREFIX}/bin")
  set(CMAKE_INSTALL_FULL_INCLUDEDIR "${CMAKE_INSTALL_PREFIX}/include")
  set(CMAKE_INSTALL_FULL_LIBDIR "${CMAKE_INSTALL_PREFIX}/lib")
else()
  include(GNUInstallDirs)
  set(DESKTOPDIR "/usr/share/applications"
      CACHE STRING "Desktop installation directory")
endif()
