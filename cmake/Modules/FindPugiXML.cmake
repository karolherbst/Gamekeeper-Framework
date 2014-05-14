# - Try to find Jsoncpp
# Once done this will define
#  PUGIXML_FOUND - System has Jsoncpp
#  PUGIXML_INCLUDE_DIRS - The Jsoncpp include directories
#  PUGIXML_LIBRARIES - The libraries needed to use Jsoncpp

include(FindPkgConfig)
pkg_check_modules(PUGIXML pugixml)

if(NOT PUGIXML_FOUND)
  find_path(PUGIXML_INCLUDE_DIRS pugixml.hpp PATH_SUFFIXES pugixml)
  find_library(PUGIXML_LIBRARIES NAMES libpugixml pugixml)
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(PUGIXML DEFAULT_MSG PUGIXML_LIBRARIES PUGIXML_INCLUDE_DIRS)
mark_as_advanced(PUGIXML_INCLUDE_DIRS PUGIXML_LIBRARIES)
