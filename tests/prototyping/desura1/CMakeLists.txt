find_package(PugiXML)
if(NOT PUGIXML_FOUND)
  add_subdirectory(pugixml)  
endif()

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${GAMEKEEPER_INCLUDE_DIR}
  ${Boost_INCLUDE_DIRS}
  ${HYPODERMIC_INCLUDE_DIRS}
  ${PUGIXML_INCLUDE_DIRS}
)

file(GLOB headers
  desuraprototype.h
)

file(GLOB sources
  desuraprototype.cpp
)

add_executable(DesuraPrototype ${sources} ${headers})
target_link_libraries(DesuraPrototype
  gamekeeperClientMain
)
