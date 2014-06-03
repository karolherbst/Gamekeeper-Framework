# - Try to find gnome-keyring
# Once done this will define
#  GNOME_KEYRING_FOUND - System has gnome-keyring
#  GNOME_KEYRING_INCLUDE_DIRS - The gnome-keyring include directories
#  GNOME_KEYRING_LIBRARIES - The libraries needed to use gnome-keyring

include(FindPkgConfig)
pkg_check_modules(GNOME_KEYRING gnome-keyring-1)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(GNOME_KEYRING DEFAULT_MSG GNOME_KEYRING_LIBRARIES GNOME_KEYRING_INCLUDE_DIRS)
mark_as_advanced(GNOME_KEYRING_INCLUDE_DIRS GNOME_KEYRING_LIBRARIES)
