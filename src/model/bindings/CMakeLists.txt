include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${GAMELIB_INCLUDE_DIR}
  ${PYTHON_INCLUDE_DIRS}
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

if(WITH_PYTHON_BINDINGS)
  add_subdirectory(python)
endif()
