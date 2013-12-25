# - Try to find Jsoncpp
# Once done this will define
#  JSONCPP_FOUND - System has Jsoncpp
#  JSONCPP_INCLUDE_DIRS - The Jsoncpp include directories
#  JSONCPP_LIBRARIES - The libraries needed to use Jsoncpp

include(FindPkgConfig)
pkg_check_modules(JSONCPP jsoncpp)

if(NOT JSONCPP_FOUND)
  find_path(JSONCPP_INCLUDE_DIRS json/json.h PATH_SUFFIXES jsoncpp)
  find_library(JSONCPP_LIBRARIES NAMES libjsoncpp jsoncpp)
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(JSONCPP DEFAULT_MSG JSONCPP_LIBRARIES JSONCPP_INCLUDE_DIRS)
mark_as_advanced(JSONCPP_INCLUDE_DIRS JSONCPP_LIBRARIES)
