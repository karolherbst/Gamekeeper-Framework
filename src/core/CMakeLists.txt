include_directories(
  ${GAMEKEEPER_PRIVATE_INCLUDE_DIR}/gamekeeper/core
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${Boost_INCLUDE_DIRS}
  ${CURL_INCLUDE_DIRS}
  ${HYPODERMIC_INCLUDE_DIRS}
  ${LOG4CPP_INCLUDE_DIR}
)

if(WIN32)
  file(GLOB platform_headers
    ${GAMEKEEPER_PRIVATE_INCLUDE_DIR}/gamekeeper/core/win32threadhelper.h
    ${GAMEKEEPER_PRIVATE_INCLUDE_DIR}/gamekeeper/core/windowsinformation.h
  )

  file(GLOB platform_sources
    win32threadhelper.cpp
    windowsinformation.cpp
  )
elseif(LINUX)
  file(GLOB platform_headers
    ${GAMEKEEPER_PRIVATE_INCLUDE_DIR}/gamekeeper/core/linuxinformation.h
    ${GAMEKEEPER_PRIVATE_INCLUDE_DIR}/gamekeeper/core/pthreadhelper.h
  )

  file(GLOB platform_sources
    linuxinformation.cpp
    pthreadhelper.cpp
  )
endif()

file(GLOB headers
  pch.h
  ${GAMEKEEPER_PRIVATE_INCLUDE_DIR}/gamekeeper/core/boostpopropertyresolver.h
  ${GAMEKEEPER_PRIVATE_INCLUDE_DIR}/gamekeeper/core/curlfiledownloader.h
  ${GAMEKEEPER_PRIVATE_INCLUDE_DIR}/gamekeeper/core/curlfiledownloaderfactory.h
  ${GAMEKEEPER_PRIVATE_INCLUDE_DIR}/gamekeeper/core/exception.h
  ${GAMEKEEPER_PRIVATE_INCLUDE_DIR}/gamekeeper/core/gnuinstalldirspaths.h
  ${GAMEKEEPER_PRIVATE_INCLUDE_DIR}/gamekeeper/core/log4cppLogger.h
  ${GAMEKEEPER_PRIVATE_INCLUDE_DIR}/gamekeeper/core/log4cpploggerFactory.h
  ${GAMEKEEPER_PRIVATE_INCLUDE_DIR}/gamekeeper/core/log4cppLoggerStream.h
  ${GAMEKEEPER_PRIVATE_INCLUDE_DIR}/gamekeeper/core/stdc++11threadmanager.h
  ${GAMEKEEPER_PRIVATE_INCLUDE_DIR}/gamekeeper/core/threadmanager.h
  ${GAMEKEEPER_PRIVATE_INCLUDE_DIR}/gamekeeper/core/xdgpaths.h
  ${platform_headers}
)

file(GLOB sources
  boostpopropertyresolver.cpp
  curlfiledownloader.cpp
  curlfiledownloaderfactory.cpp
  exception.cpp
  gnuinstalldirspaths.cpp
  log4cppLogger.cpp
  log4cpploggerFactory.cpp
  log4cppLoggerStream.cpp
  stdc++11threadmanager.cpp
  xdgpaths.cpp
  ${platform_sources}
)

set_source_files_properties(gnuinstalldirspaths.cpp
                            PROPERTIES COMPILE_DEFINITIONS DATADIR="${CMAKE_INSTALL_FULL_DATADIR}")

add_library(gamekeeperCore SHARED ${sources} ${headers})
target_link_libraries(gamekeeperCore
  ${Boost_FILESYSTEM_LIBRARY}
  ${CURL_LIBRARIES}
  ${LOG4CPP_LIBRARIES}
  ${Boost_SYSTEM_LIBRARY}
  ${CMAKE_THREAD_LIBS_INIT}
)

if(BUILD_USE_COTIRE)
  cotire(gamekeeperCore)
endif()

if(MINGW)
  add_definitions(-DBOOST_THREAD_USE_LIB)
  target_link_libraries(gamekeeperCore
    ${Boost_CHRONO_LIBRARY}
    ${Boost_THREAD_LIBRARY}
  )
endif()
