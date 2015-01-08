include(${CSHARP_USE_FILE})

add_gamekeeper_swig_binding(csharp GameKeeperModelCSharp Model -namespace org.gamekeeper.bindings.model)

set(CSHARP_BINARY_DIRECTORY  ${CMAKE_BINDINGS_OUT_DIR}/csharp)
csharp_add_library(GameKeeperModelCSharp
  ""
  "${SWIG_csharp_MODULE_PATH}/*.cs"
)
