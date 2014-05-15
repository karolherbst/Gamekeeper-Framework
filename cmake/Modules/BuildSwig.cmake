ExternalProject_Add(
  swig
  URL ${SWIG_URL}
  URL_MD5 ${SWIG_MD5}
  UPDATE_COMMAND ""
  CONFIGURE_COMMAND ""
  BUILD_COMMAND ""
  INSTALL_COMMAND ""
)

ExternalProject_Get_Property(
  swig
  source_dir
)

set(SWIG_DIR ${source_dir})
set(SWIG_EXECUTABLE ${SWIG_DIR}/swig.exe)
set(SWIG_USE_FILE ${CMAKE_ROOT}/Modules/UseSWIG.cmake)
