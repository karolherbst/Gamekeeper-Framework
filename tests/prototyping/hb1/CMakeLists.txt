include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${GAMELIB_INCLUDE_DIR}
  ${CURL_INCLUDE_DIRS}
  ${PYTHON_INCLUDE_DIRS}
)

file(GLOB headers
  game.h
  hbprototype.h
)

file(GLOB py_sources
  game.cpp
)

file(GLOB sources
  hbprototype.cpp
)

add_library(GameLibPy SHARED ${py_sources})

target_link_libraries(GameLibPy
  ${PYTHON_LIBRARIES}
)

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
