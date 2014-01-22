include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${GAMELIB_INCLUDE_DIR}
)

file(GLOB headers
  pch.h
  gameBinding.h
)

file(GLOB sources
  gameBinding.cpp
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

macro(add_gamelib_swig_binding type name)
  set_source_files_properties(${swig_sources} PROPERTIES CPLUSPLUS ON)
  set_source_files_properties(${swig_sources} PROPERTIES SWIG_FLAGS "-DGAMELIB_BINDING_MODULE=${name};${ARGN}")
  swig_add_module(${name} ${type} ${swig_sources})
  swig_link_libraries(${name} gamelibModelBindings)
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
