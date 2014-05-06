if(MINGW)
  set(Boost_CONFIGURE_COMMAND "./bootstrap.sh")
  set(Boost_CONFIGURE_FLAGS "--with-toolset=mingw")
  set(Boost_BJAM_BINARY "./bjam.exe")
  set(Boost_BJAM_FLAGS toolset=gcc)
elseif(WIN32)
  set(Boost_CONFIGURE_COMMAND "bootstrap.bat")
  set(BJAM_BINARY "b2.exe")
else()
  message("BuildBoost isn't supported on this plattform")
endif()

set(Boost_CONFIGURE_LIBS --with-libraries=system,filesystem,signals,program_options)

ExternalProject_Add(
  boost
  URL "${BOOST_URL}"
  URL_MD5 ${BOOST_MD5}
  UPDATE_COMMAND ""
  BUILD_IN_SOURCE 1
  CONFIGURE_COMMAND ${Boost_CONFIGURE_COMMAND} ${Boost_CONFIGURE_FLAGS} ${Boost_CONFIGURE_LIBS}
  BUILD_COMMAND ${Boost_BJAM_BINARY} ${Boost_BJAM_FLAGS} --layout=tagged  variant=release link=shared threading=multi
    runtime-link=shared
  INSTALL_COMMAND ""
)
set(Boost_LIB_ADD_STRING "mt")

ExternalProject_Get_Property(
  boost
  source_dir
)

set(Boost_DIR ${source_dir})
set(Boost_INCLUDE_DIRS ${Boost_DIR})
set(Boost_LIBRARY_DIRS ${Boost_DIR}/stage/lib)

if(MINGW)
  set(Boost_SUFFIX dll.a)
else()
  set(Boost_SUFFIX lib)
endif()

set(Boost_FILESYSTEM_LIBRARY "${Boost_LIBRARY_DIRS}/libboost_filesystem-${Boost_LIB_ADD_STRING}.${Boost_SUFFIX}")
set(Boost_PROGRAM_OPTIONS_LIBRARY "${Boost_LIBRARY_DIRS}/libboost_program_options-${Boost_LIB_ADD_STRING}.${Boost_SUFFIX}")
set(Boost_SYSTEM_LIBRARY "${Boost_LIBRARY_DIRS}/libboost_system-${Boost_LIB_ADD_STRING}.${Boost_SUFFIX}")
