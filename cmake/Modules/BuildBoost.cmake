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

set(Boost_CONFIGURE_LIBS --with-libraries=system,filesystem,signals)

ExternalProject_Add(
  boost
  URL "${Boost_URL}"
  URL_MD5 ${Boost_MD5}
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
  set(Boost_SUFFIX a)
else()
  set(Boost_SUFFIX lib)
endif()

set(Boost_CHRONO_LIBRARY "${Boost_LIBRARY_DIR}/libboost_chrono-${Boost_LIB_ADD_STRING}.${Boost_SUFFIX}")
set(Boost_DATE_TIME_LIBRARY "${Boost_LIBRARY_DIR}/libboost_date_time-${Boost_LIB_ADD_STRING}.${Boost_SUFFIX}")
set(Boost_FILESYSTEM_LIBRARY "${Boost_LIBRARY_DIR}/libboost_filesystem-${Boost_LIB_ADD_STRING}.${Boost_SUFFIX}")
set(Boost_PRG_EXEC_MONITOR_LIBRARY "${Boost_LIBRARY_DIR}/libboost_prg_exec_monitor-${Boost_LIB_ADD_STRING}.${Boost_SUFFIX}")
set(Boost_SYSTEM_LIBRARY "${Boost_LIBRARY_DIR}/libboost_system-${Boost_LIB_ADD_STRING}.${Boost_SUFFIX}")
set(Boost_TEST_EXEC_MONITOR_LIBRARY "${Boost_LIBRARY_DIR}/libboost_test_exec_monitor-${Boost_LIB_ADD_STRING}.${Boost_SUFFIX}")
set(Boost_THREAD_LIBRARY "${Boost_LIBRARY_DIR}/libboost_thread-${Boost_LIB_ADD_STRING}.${Boost_SUFFIX}")
set(Boost_UNIT_TEST_FRAMEWORK_LIBRARY "${Boost_LIBRARY_DIR}/libboost_unit_test_framework-${Boost_LIB_ADD_STRING}.${Boost_SUFFIX}")

set_property(TARGET boost PROPERTY FOLDER "ThirdParty")