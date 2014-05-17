# - Try to find Jansson
# Once done this will define
#  JANSSON_FOUND - System has Jansson
#  JANSSON_INCLUDE_DIRS - The Jansson include directories
#  JANSSON_LIBRARIES - The libraries needed to use Jansson

include(FindPkgConfig)
pkg_check_modules(JANSSON jansson)

if(NOT JANSSON_FOUND)
  find_path(JANSSON_INCLUDE_DIRS jansson.h PATH_SUFFIXES jansson)
  find_library(JANSSON_LIBRARIES NAMES libjansson jansson)
elseif(NOT JANSSON_INCLUDE_DIRS)
  set(JANSSON_INCLUDE_DIRS "${JANSSON_INCLUDEDIR}")
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(JANSSON DEFAULT_MSG JANSSON_LIBRARIES JANSSON_INCLUDE_DIRS)
mark_as_advanced(JANSSON_INCLUDE_DIRS JANSSON_LIBRARIES)
