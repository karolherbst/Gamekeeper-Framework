include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${GAMELIB_INCLUDE_DIR}
)

file(GLOB headers
  pch.h
  game.h
)

file(GLOB sources
  game.cpp
)

set(swig_sources
  ../game.i
)

add_library(gamelibModelBindings SHARED ${sources} ${headers})
install_library(gamelibModelBindings)

# only include SWIG if we build our bindings
if(WITH_CSHARP_BINDINGS OR WITH_JAVA_BINDINGS OR WITH_PYTHON_BINDINGS)
  set(CMAKE_SWIG_FLAGS -c++ -includeall)
  include(${SWIG_USE_FILE})
endif()

macro(add_gamelib_swig_binding type name module)
  set(swigName ${type}_${module})
  set(SWIG_${swigName}_MODULE_PATH ${CMAKE_GEN_DIR}/bindings/${type}/${module} CACHE STRING "The path to the generated ${type} sources")
  set(SWIG_${swigName}_LIBRARY_PATH ${CMAKE_BINDINGS_OUT_DIR}/${type}/${module} CACHE STRING "The path to the compiled ${type} native binary")
  set(CMAKE_SWIG_OUTDIR ${SWIG_${swigName}_MODULE_PATH})
  set_source_files_properties(${swig_sources} PROPERTIES CPLUSPLUS ON)
  set_source_files_properties(${swig_sources} PROPERTIES SWIG_FLAGS "-DGAMELIB_BINDING_MODULE=${name};${ARGN}")
  swig_add_module(${name} ${type} ${swig_sources})
  swig_link_libraries(${name} gamelib${module}Bindings)
  set_target_properties(${SWIG_MODULE_${name}_REAL_NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${SWIG_${swigName}_LIBRARY_PATH}")
endmacro()

if(WITH_CSHARP_BINDINGS)
  add_subdirectory(csharp)
endif()

if(WITH_PYTHON_BINDINGS)
  add_subdirectory(python)
endif()

if(WITH_JAVA_BINDINGS)
  add_subdirectory(java)
endif()
