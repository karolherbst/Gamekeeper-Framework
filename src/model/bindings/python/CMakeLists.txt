include_directories(
  ${PYTHON_INCLUDE_DIRS}
)

# builtin gives us a performance boost
# http://www.swig.org/Doc2.0/Python.html#Python_builtin_types
set(GAMELIB_BINDING_MODULE_NAME GameLibPy)
set_source_files_properties(${swig_sources} PROPERTIES CPLUSPLUS ON)
set_source_files_properties(${swig_sources} PROPERTIES SWIG_FLAGS "-builtin;-DGAMELIB_BINDING_MODULE=${GAMELIB_BINDING_MODULE_NAME}")
swig_add_module(GameLibPy python ${swig_sources})
swig_link_libraries(GameLibPy gamelibModelBindings)
