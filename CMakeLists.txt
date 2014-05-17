#######################################################################################################################
# list of all option used in this project
#  name                 default      arch     description
#
# global Object documentation
#  BUILD_TESTS          OFF          all      enables the build of unit tests
#  BUILD_PROTOTYPES     OFF          all      enables the build of prototypes
#  GENERATE_DOCS        OFF          all      generates documentation (needs doxygen)
#  INSTALL_DOCS         OFF          all      installs the generated documentation
#
# control of bindings
#  WITH_CSHARP_BINDINGS OFF          all      enables C# bindings (currently broken)
#  WITH_JAVA_BINDINGS   OFF          all      enables Java bindings
#  WITH_PYTHON_BINDINGS OFF          all      enables Python bindings
#
# flags for external libraries
#  BOOST_URL                         win/mac  Path to the boost archive
#  BOOST_MD5                         win/mac  md5sum of the used archive
#  CURL_URL                          win      Path to the curl archive
#  CURL_MD5                          win      md5sum of the used archive
#  GTEST_URL                         all      Path to the Gtest archive
#  GTEST_MD5                         all      md5sum of the used archive
#  JSONCPP_URL                       win/mac  Path to the JsonCPP archive
#  JSONCPP_MD5                       win/mac  md5sum of the used archive
#######################################################################################################################

project(gamekeeper)
cmake_minimum_required(VERSION 2.8)

#######################################################################################################################
# some global stuff
#######################################################################################################################

# Path where external dependencies will be installed
set(CMAKE_EXTERNAL_BINARY_DIR "${CMAKE_BINARY_DIR}/external")
set(CMAKE_COMPILER_PATH "${CMAKE_SOURCE_DIR}/cmake/Compiler")
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/Modules")
set(CMAKE_PATCH_DIR "${CMAKE_SOURCE_DIR}/cmake/Patches")
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

if(WIN32)
  add_definitions(-DUNICODE -D_UNICODE)
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

# threads
find_package(Threads REQUIRED)
if(CMAKE_USE_PTHREADS_INIT)
  add_definitions(-DGAMEKEEPER_PTHREAD)
endif()
if(CMAKE_USE_WIN32_THREADS_INIT)
  add_definitions(-DGAMEKEEPER_WIN32_THREAD)
endif()

# we need to patch some deps on MinGW
if(MINGW)
  find_package(Patch REQUIRED)
endif()

find_package(Hypodermic)
if(NOT HYPODERMIC_FOUND)
  list(APPEND boost_ADDITIONAL_COMPONENTS signals)
endif()

if(MINGW)
  list(APPEND boost_ADDITIONAL_COMPONENTS chrono thread)
endif()

# boost
find_package(Boost 1.44 COMPONENTS filesystem program_options system ${boost_ADDITIONAL_COMPONENTS} ${REQUIRED_ON_LINUX})
if(NOT Boost_FOUND)
  set_download_url(BOOST http://sourceforge.net/projects/boost/files/boost/1.55.0/boost_1_55_0.zip/download 8aca361a4713a1f491b0a5e33fee0f1f)
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

# jansson
find_package(Jansson ${REQUIRED_ON_LINUX})
if(NOT JANSSON_FOUND)
  set_download_url(JANSSON http://www.digip.org/jansson/releases/jansson-2.6.tar.gz 00dd7b55c01c74cac59df398208b92ed)
  include(BuildJansson)
endif()

# jsoncpp
find_package(Jsoncpp ${REQUIRED_ON_LINUX})
if(NOT JSONCPP_FOUND)
  set_download_url(JSONCPP http://sourceforge.net/projects/jsoncpp/files/jsoncpp/0.6.0-rc2/jsoncpp-src-0.6.0-rc2.tar.gz/download 363e2f4cbd3aeb63bf4e571f377400fb)
  include(BuildJSONCpp)
endif()

# log4cpp
find_package(log4cpp ${REQUIRED_ON_LINUX})
if(NOT LOG4CPP_FOUND)
  set_download_url(LOG4CPP http://sourceforge.net/projects/log4cpp/files/log4cpp-1.1.x%20%28new%29/log4cpp-1.1/log4cpp-1.1.1.tar.gz/download 1e173df8ee97205f412ff84aa93b8fbe)
  include(Buildlog4cpp)
endif()

# pugixml
find_package(PugiXML)

# swig
if(WITH_CSHARP_BINDINGS OR WITH_JAVA_BINDINGS OR WITH_PYTHON_BINDINGS)
  find_package(SWIG ${REQUIRED_ON_UNIX})
  if(NOT SWIG_FOUND)
    set_download_url(SWIG http://downloads.sourceforge.net/project/swig/swigwin/swigwin-3.0.0/swigwin-3.0.0.zip c1d85fee2421d4edac552923fc4e5bde)
    include(BuildSwig)
  endif()
  
  if(WITH_CSHARP_BINDINGS)
    find_package(CSharp REQUIRED)
  endif()
  
  if(WITH_JAVA_BINDINGS)
    find_package(Java REQUIRED)
    find_package(JNI REQUIRED)
  endif()
  
  if(WITH_PYTHON_BINDINGS)
    set(Python_ADDITIONAL_VERSIONS 3.4)
    find_package(PythonLibs 3 REQUIRED)
  endif()
endif()

#######################################################################################################################
# set some cmake variables needed by GameKeeper build
#######################################################################################################################

set(GAMEKEEPER_PRIVATE_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include_private")
set(GAMEKEEPER_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include")

#######################################################################################################################
# build GameKeeper Framework
#######################################################################################################################

add_subdirectory(doc)
add_subdirectory(third_party)
add_subdirectory(include_private)
add_subdirectory(include)
add_subdirectory(src)
add_subdirectory(tests)
