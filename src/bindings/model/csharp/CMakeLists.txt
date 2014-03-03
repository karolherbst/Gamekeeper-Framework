include(${CSHARP_USE_FILE})

add_gamekeeper_swig_binding(csharp GameLibModelCSharp Model -namespace org.gamelib.bindings.model)

set(CSHARP_BINARY_DIRECTORY  ${CMAKE_BINDINGS_OUT_DIR}/csharp)
csharp_add_library(GameLibModel
  ""
  "${SWIG_csharp_MODULE_PATH}/*.cs"
)
