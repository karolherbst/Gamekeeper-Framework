if(GCC)
  set(Boost_BJAM_USING gcc)
elseif(CLANG)
  set(Boost_BJAM_USING clang)
elseif(MSVC)
  set(Boost_BJAM_USING msvc)
endif()

if(MINGW)
  set(Boost_CONFIGURE_COMMAND "./bootstrap.sh")
  set(Boost_BJAM_BINARY "./bjam")
  set(Boost_BJAM_FLAGS target-os=windows threadapi=win32)
elseif(WIN32)
  set(Boost_CONFIGURE_COMMAND "bootstrap.bat")
  set(BJAM_BINARY "b2.exe")
else()
  message("BuildBoost isn't supported on this plattform")
endif()

set(Boost_CONFIGURE_LIBS --with-libraries=system,filesystem,signals,program_options)

# use % as ; seperator, so that we can generate a valid jam file
ExternalProject_Add(
  boost
  URL "${BOOST_URL}"
  URL_MD5 ${BOOST_MD5}
  LIST_SEPARATOR %
  UPDATE_COMMAND ""
  BUILD_IN_SOURCE 1
  CONFIGURE_COMMAND ""
  BUILD_COMMAND ${Boost_BJAM_BINARY} ${Boost_BJAM_FLAGS} --layout=tagged  variant=release link=shared threading=multi
    runtime-link=shared --user-config=user-config.jam
  INSTALL_COMMAND ""
)

# create jam file
ExternalProject_Add_Step(
  boost
  jamfile
  COMMAND ${CMAKE_COMMAND} -E echo "using ${Boost_BJAM_USING} : : ${CMAKE_CXX_COMPILER} : <rc>${CMAKE_RC_COMPILER} <archiver>${CMAKE_AR} <ranlib>${CMAKE_RANLIB} %" > <SOURCE_DIR>/user-config.jam
  DEPENDEES download
  DEPENDERS configure
)

ExternalProject_Add_Step(
  boost
  build_bjam
  COMMAND ${Boost_CONFIGURE_COMMAND} ${Boost_CONFIGURE_LIBS}
  DEPENDEES jamfile
  DEPENDERS build
  WORKING_DIRECTORY "<SOURCE_DIR>"
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

set(Boost_CHRONO_LIBRARY "${Boost_LIBRARY_DIRS}/libboost_chrono-${Boost_LIB_ADD_STRING}.${Boost_SUFFIX}")
set(Boost_FILESYSTEM_LIBRARY "${Boost_LIBRARY_DIRS}/libboost_filesystem-${Boost_LIB_ADD_STRING}.${Boost_SUFFIX}")
set(Boost_PROGRAM_OPTIONS_LIBRARY "${Boost_LIBRARY_DIRS}/libboost_program_options-${Boost_LIB_ADD_STRING}.${Boost_SUFFIX}")
set(Boost_SYSTEM_LIBRARY "${Boost_LIBRARY_DIRS}/libboost_system-${Boost_LIB_ADD_STRING}.${Boost_SUFFIX}")
set(Boost_THREAD_LIBRARY "${Boost_LIBRARY_DIRS}/libboost_thread-${Boost_LIB_ADD_STRING}.${Boost_SUFFIX}")
