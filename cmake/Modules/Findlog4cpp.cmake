# - Try to find log4cpp
# Once done this will define
#  LOG4CPP_FOUND - System has Jsoncpp
#  LOG4CPP_INCLUDE_DIRS - The Jsoncpp include directories
#  LOG4CPP_LIBRARIES - The libraries needed to use Jsoncpp

include(FindPkgConfig)
pkg_check_modules(LOG4CPP log4cpp)

if(NOT LOG4CPP_FOUND)
  find_path(LOG4CPP_INCLUDE_DIRS log4cpp/Category.hh PATH_SUFFIXES log4cpp)
  find_library(LOG4CPP_LIBRARIES NAMES liblog4cpp log4cpp)
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(LOG4CPP DEFAULT_MSG LOG4CPP_LIBRARIES)
mark_as_advanced(LOG4CPP_INCLUDE_DIRS LOG4CPP_LIBRARIES)
