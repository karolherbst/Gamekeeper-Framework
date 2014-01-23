include_directories(
  ${PYTHON_INCLUDE_DIRS}
)

# builtin gives us a performance boost
# http://www.swig.org/Doc2.0/Python.html#Python_builtin_types
add_gamelib_swig_binding(python GameLibPy -builtin)
# we copy the generated python file for now, later we should use compiled python modules
add_custom_command(
  TARGET ${SWIG_MODULE_GameLibPy_REAL_NAME}
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_if_different "${SWIG_python_MODULE_PATH}/GameLibPy.py" "${CMAKE_BINDINGS_OUT_DIR}/python/"
)
