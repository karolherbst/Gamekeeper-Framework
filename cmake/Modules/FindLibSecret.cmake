# - Try to find libsecret
# Once done this will define
#  LIBSECRET_FOUND - System has libsecret
#  LIBSECRET_INCLUDE_DIRS - The libsecret include directories
#  LIBSECRET_LIBRARIES - The libraries needed to use libsecret

include(FindPkgConfig)
pkg_check_modules(LIBSECRET libsecret-1)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(LIBSECRET DEFAULT_MSG LIBSECRET_LIBRARIES LIBSECRET_INCLUDE_DIRS)
mark_as_advanced(LIBSECRET_INCLUDE_DIRS LIBSECRET_LIBRARIES)
