# - Try to find cwidget
# Once done this will define
#  CWIDGET_FOUND - System has cwidget
#  CWIDGET_INCLUDE_DIRS - The cwidget include directories
#  CWIDGET_LIBRARIES - The libraries needed to use cwidget

include(FindPkgConfig)
pkg_check_modules(CWIDGET cwidget)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(CWIDGET DEFAULT_MSG CWIDGET_LIBRARIES CWIDGET_INCLUDE_DIRS)
mark_as_advanced(CWIDGET_INCLUDE_DIRS CWIDGET_LIBRARIES)
