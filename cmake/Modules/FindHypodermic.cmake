# - Try to find Hypodermic
# Once done this will define
#  HYPODERMIC_FOUND - System has Hypodermic
#  HYPODERMIC_INCLUDE_DIRS - The Hypodermic include directories
#  HYPODERMIC_LIBRARIES - The libraries needed to use Hypodermic

find_path(HYPODERMIC_INCLUDE_DIRS Hypodermic/ContainerBuilder.h)
find_library(HYPODERMIC_LIBRARIES NAMES libhypodermic hypodermic)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(HYPODERMIC DEFAULT_MSG HYPODERMIC_LIBRARIES HYPODERMIC_INCLUDE_DIRS)
mark_as_advanced(HYPODERMIC_INCLUDE_DIRS HYPODERMIC_LIBRARIES)
