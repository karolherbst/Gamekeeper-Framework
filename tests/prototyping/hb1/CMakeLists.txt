include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${GAMELIB_INCLUDE_DIR}
  ${HYPODERMIC_INCLUDE_DIRS}
  ${LOG4CPP_INCLUDE_DIR}
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
  -DPYTHONPATH="${CMAKE_CURRENT_SOURCE_DIR}"
  -DGAMELIBPYPATH="${CMAKE_BINARY_DIR}/src/model/bindings/python")

add_executable(HBPrototype ${sources} ${headers} ${otherFiles})
target_link_libraries(HBPrototype
  gamelibClient
  gamelibModelBindings
  ${LOG4CPP_LIBRARIES}
  ${PYTHON_LIBRARIES}
)
