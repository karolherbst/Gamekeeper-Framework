include_directories(
  ${JNI_INCLUDE_DIRS}
)

set(GAMELIB_BINDING_MODULE_NAME GameLibJava)
set_source_files_properties(${swig_sources} PROPERTIES CPLUSPLUS ON)
set_source_files_properties(${swig_sources} PROPERTIES SWIG_FLAGS -DGAMELIB_BINDING_MODULE=${GAMELIB_BINDING_MODULE_NAME})
swig_add_module(GameLibJava java ${swig_sources})
swig_link_libraries(GameLibJava gamelibModelBindings)
