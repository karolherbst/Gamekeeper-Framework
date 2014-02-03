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
set(CMAKE_GEN_DIR "${CMAKE_BINARY_DIR}/gen")
set(CMAKE_BINDINGS_OUT_DIR "${CMAKE_BINARY_DIR}/bindings")

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

find_package(Hypodermic)
if(NOT HYPODERMIC_FOUND)
  set(boost_ADDITIONAL_COMPONENTS signals)
endif()

# boost
find_package(Boost 1.44 COMPONENTS filesystem system ${boost_ADDITIONAL_COMPONENTS} ${REQUIRED_ON_LINUX})
if(NOT Boost_FOUND)
  set_download_url(Boost http://sourceforge.net/projects/boost/files/boost/1.55.0/boost_1_55_0.zip/download 8aca361a4713a1f491b0a5e33fee0f1f)
  include(BuildBoost)
endif()

# hypdermic
if(NOT HYPODERMIC_FOUND)
  include(BuildHypodermic)
endif()

# curl
find_package(CURL ${REQUIRED_ON_UNIX})
if(NOT CURL_FOUND)
  set_download_url(CURL http://curl.haxx.se/download/curl-7.35.0.zip 4166537736ae9fb85c6a9aa6484dee84)
  include(BuildCURL)
endif()

# gtest
if(BUILD_TESTS)
  enable_testing()
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
    find_package(CSharp REQUIRED)
  endif()
  
  if(WITH_JAVA_BINDINGS)
    find_package(Java REQUIRED)
    find_package(JNI REQUIRED)
  endif()
  
  if(WITH_PYTHON_BINDINGS)
    find_package(PythonLibs REQUIRED)
  endif()
endif()

#######################################################################################################################
# set some cmake variables needed by gamelib build
#######################################################################################################################

set(GAMELIB_PRIVATE_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include_private")
set(GAMELIB_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include")

#######################################################################################################################
# build libgamelib
#######################################################################################################################

add_subdirectory(include_private)
add_subdirectory(include)
add_subdirectory(src)
add_subdirectory(tests)
