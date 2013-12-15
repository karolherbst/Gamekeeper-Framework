include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${GAMELIB_INCLUDE_DIR}
  ${CURL_INCLUDE_DIRS}
  ${PYTHON_INCLUDE_DIRS}
)

file(GLOB headers
  game.h
  gameBind.h
  hbprototype.h
)

file(GLOB binding_sources
  game.cpp
)

file(GLOB sources
  hbprototype.cpp
)

add_library(GameModelBindings SHARED ${binding_sources} ${headers})

include(${SWIG_USE_FILE})
# builtin gives us a performance boost
# http://www.swig.org/Doc2.0/Python.html#Python_builtin_types
set(CMAKE_SWIG_FLAGS -c++ -py3 -builtin)

set_source_files_properties(game.i PROPERTIES CPLUSPLUS ON)
set_source_files_properties(game.i PROPERTIES SWIG_FLAGS "-includeall")
swig_add_module(GameLibPy python game.i)
swig_link_libraries(GameLibPy GameModelBindings)

get_target_property(target_loc GameLibPy LOCATION)
get_filename_component(target_path "${target_loc}" PATH)

file(GLOB otherFiles
  hbprototype.py
)

add_definitions(
  -DPYMODULENAME="hbprototype"
  -DPYTHONPATH="${CMAKE_CURRENT_SOURCE_DIR}"
  -DGAMELIBPYPATH="${target_path}")

add_executable(HBPrototype ${sources} ${headers} ${otherFiles})
target_link_libraries(HBPrototype
  gamelibClient
  ${CURL_LIBRARIES}
  ${PYTHON_LIBRARIES}
)
