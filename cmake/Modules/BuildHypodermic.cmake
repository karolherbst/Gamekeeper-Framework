set(HYPODERMIC_INSTALL_DIR ${CMAKE_EXTERNAL_BINARY_DIR}/hypodermic)

ExternalProject_Add(
  hypodermic
  URL https://github.com/karolherbst/hypodermic/archive/master.zip
  CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${HYPODERMIC_INSTALL_DIR} -DBT=SHARED -DBOOST_INCLUDEDIR=${Boost_INCLUDE_DIRS} -DBOOST_LIBRARYDIR=${Boost_LIBRARY_DIRS}
  BUILD_IN_SOURCE 0
  SOURCE_DIR <prefix>/src/<name>/hypodermic-master/
)

set(HYPODERMIC_LIBRARIES ${HYPODERMIC_INSTALL_DIR}/lib/libhypodermic.so)
set(HYPODERMIC_INCLUDE_DIRS ${HYPODERMIC_INSTALL_DIR}/include)
