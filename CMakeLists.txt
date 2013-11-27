#######################################################################################################################
# global Object documentation
# name              default      description
# BUILD_TESTS       OFF          enables the build of unit tests
# BUILD_PROTOTYPES  OFF          enables the build of prototypes
#######################################################################################################################

project(gamelib CXX)
cmake_minimum_required(VERSION 2.8)

#######################################################################################################################
# some global stuff
#######################################################################################################################

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/Modules")

set(LINUX FALSE)
if(NOT WIN32)
  if(NOT APPLE)
    set(LINUX TRUE)
    set(REQUIRED_ON_LINUX REQUIRED)
  endif()
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

find_package(Boost 1.44 COMPONENTS filesystem ${REQUIRED_ON_LINUX})
find_package(CURL ${REQUIRED_ON_UNIX})

# gtest
if(BUILD_TESTS)
  find_package(GTest)
  
  if(NOT GTEST_FOUND)
    set_download_url(GTEST https://googletest.googlecode.com/files/gtest-1.7.0.zip 2d6ec8ccdf5c46b05ba54a9fd1d130d7)
    include(BuildGTest)
  endif()
endif()

#######################################################################################################################
# set some cmake variables needed by gamelib build
#######################################################################################################################

set(GAMELIB_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include")

#######################################################################################################################
# build libgamelib
#######################################################################################################################

add_subdirectory(src)
