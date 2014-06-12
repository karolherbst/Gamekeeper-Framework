#######################################################################################################################
# global options
#######################################################################################################################

option(BUILD_TESTS "enables the build of unit tests" OFF)
option(BUILD_PROTOTYPES "enables the build of prototypes" OFF)

option(WITH_GNOME_KEYRING "enables the gnome-keyring authmanager" OFF)

#######################################################################################################################
# doc options
#######################################################################################################################

option(GENERATE_DOCS "generates documentation" OFF)
option(INSTALL_DOCS "installs the generated documentation" OFF)

#######################################################################################################################
# module options
#######################################################################################################################

option(WITH_CSHARP_BINDINGS "compile Python bindings" OFF)
option(WITH_JAVA_BINDINGS "compile Java bindings" OFF)
option(WITH_PYTHON_BINDINGS "compile Python bindings" OFF)
