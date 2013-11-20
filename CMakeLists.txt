project(gamelib CXX)
cmake_minimum_required(VERSION 2.8)

#######################################################################################################################
# some global stuff
#######################################################################################################################

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/Modules")

if(NOT WIN32)
  set(REQUIRED_ON_UNIX REQUIRED)
endif()

#######################################################################################################################
# include some cmake modules
#######################################################################################################################

include(ExternalProject)
include(Macros)
include(Options)
include(SetInstallPaths)

#######################################################################################################################
# dependency management
#######################################################################################################################

find_package(Boost 1.20 ${REQUIRED_ON_UNIX})
find_package(CURL ${REQUIRED_ON_UNIX})

# gtest
if(ENABLE_TEST)
  find_package(GTest)
  
  if(NOT GTEST_FOUND)
    set_download_url(GTEST https://googletest.googlecode.com/files/gtest-1.7.0.zip 2d6ec8ccdf5c46b05ba54a9fd1d130d7)
    include(BuildGTest)
  endif()
endif()

#######################################################################################################################
# build libgamelib
#######################################################################################################################

add_subdirectory(src)
