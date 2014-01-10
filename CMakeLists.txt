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

# Path where external dependencies will be installed
set(CMAKE_EXTERNAL_BINARY_DIR "${CMAKE_BINARY_DIR}/external")
set(CMAKE_COMPILER_PATH "${CMAKE_SOURCE_DIR}/cmake/Compiler")
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

# macros should be included first, then environment, the others aren't that important
include(Macros)
include(Environment)
include(ExternalProject)
include(Options)
include(SetInstallPaths)

#######################################################################################################################
# dependency management
#######################################################################################################################

# boost
find_package(Boost 1.44 COMPONENTS filesystem system ${REQUIRED_ON_LINUX})

# curl
find_package(CURL ${REQUIRED_ON_UNIX})

# hypdermic
find_package(Hypodermic)
if(NOT HYPODERMIC_FOUND)
  include(BuildHypodermic)
endif()

# gtest
if(BUILD_TESTS)
  find_package(GTest)
  
  if(NOT GTEST_FOUND)
    set_download_url(GTEST https://googletest.googlecode.com/files/gtest-1.7.0.zip 2d6ec8ccdf5c46b05ba54a9fd1d130d7)
    include(BuildGTest)
  endif()
endif()

# log4cpp
find_package(log4cpp ${REQUIRED_ON_LINUX})

# swig
if(WITH_CSHARP_BINDINGS OR WITH_JAVA_BINDINGS OR WITH_PYTHON_BINDINGS)
  find_package(SWIG REQUIRED)
  
  if(WITH_CSHARP_BINDINGS)
    # find file needed for mono/.NET
  endif()
  
  if(WITH_JAVA_BINDINGS)
    find_package(Java REQUIRED)
  endif()
  
  if(WITH_PYTHON_BINDINGS)
    find_package(PythonLibs REQUIRED)
  endif()
endif()

#######################################################################################################################
# set some cmake variables needed by gamelib build
#######################################################################################################################

set(GAMELIB_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include")

#######################################################################################################################
# build libgamelib
#######################################################################################################################

add_subdirectory(include)
add_subdirectory(src)
add_subdirectory(tests)
