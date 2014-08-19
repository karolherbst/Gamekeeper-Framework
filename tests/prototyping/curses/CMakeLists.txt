find_package(CWidget REQUIRED)

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${GAMEKEEPER_PRIVATE_INCLUDE_DIR}
  ${GAMEKEEPER_INCLUDE_DIR}
  ${CWIDGET_INCLUDE_DIRS}
  ${HYPODERMIC_INCLUDE_DIRS}
)

file(GLOB headers
  cursesprototype.h
)

file(GLOB sources
  cursesprototype.cpp
)

add_executable(CursesPrototype ${sources} ${headers})
target_link_libraries(CursesPrototype
  gamekeeperClientMain
  ${CWIDGET_LIBRARIES}
)

if(BUILD_USE_COTIRE)
  cotire(CursesPrototype)
endif()
