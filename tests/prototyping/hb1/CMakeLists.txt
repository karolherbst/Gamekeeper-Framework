include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${GAMELIB_INCLUDE_DIR}
  ${CURL_INCLUDE_DIRS}
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
  -DPYTHONPATH="${CMAKE_CURRENT_SOURCE_DIR}")

add_executable(HBPrototype ${sources} ${headers} ${otherFiles})
target_link_libraries(HBPrototype
  gamelibClient
  ${CURL_LIBRARIES}
  ${PYTHON_LIBRARIES}
)
