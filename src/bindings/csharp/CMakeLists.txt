include(${CSHARP_USE_FILE})

add_gamelib_swig_binding(csharp GameLibCSharp -namespace org.gamelib.bindings)

set(CSHARP_BINARY_DIRECTORY  ${CMAKE_BINDINGS_OUT_DIR}/csharp)
csharp_add_library(GameLib
  ""
  "${SWIG_csharp_MODULE_PATH}/*.cs"
)
