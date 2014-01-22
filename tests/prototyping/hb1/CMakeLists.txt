include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${GAMELIB_INCLUDE_DIR}
  ${HYPODERMIC_INCLUDE_DIRS}
  ${PYTHON_INCLUDE_DIRS}
)

file(GLOB headers
  hbprototype.h
)

file(GLOB sources
  hbprototype.cpp
)

file(GLOB otherFiles
  hbprototype.py
)

add_definitions(
  -DPYMODULENAME="hbprototype"
  -DPYMODULEPATH="${CMAKE_CURRENT_SOURCE_DIR}"
  -DPYTHONPATH="${CMAKE_BINDINGS_OUT_DIR}/python/"
)

add_executable(HBPrototype ${sources} ${headers} ${otherFiles})
target_link_libraries(HBPrototype
  gamelibClient
  gamelibModelBindings
  ${PYTHON_LIBRARIES}
)
