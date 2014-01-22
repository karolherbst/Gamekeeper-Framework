include_directories(
  ${JNI_INCLUDE_DIRS}
)

set_source_files_properties(${swig_sources} PROPERTIES CPLUSPLUS ON)
swig_add_module(GameLibJava java ${swig_sources})
swig_link_libraries(GameLibJava gamelibModelBindings)
