include_directories(
  ${PYTHON_INCLUDE_DIRS}
)

# builtin gives us a performance boost
# http://www.swig.org/Doc2.0/Python.html#Python_builtin_types
add_gamelib_swig_binding(python GameLibPy -builtin)
