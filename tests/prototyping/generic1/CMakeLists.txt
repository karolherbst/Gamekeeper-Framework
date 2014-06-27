include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${GAMEKEEPER_PRIVATE_INCLUDE_DIR}
  ${GAMEKEEPER_INCLUDE_DIR}
  ${Boost_INCLUDE_DIRS}
  ${HYPODERMIC_INCLUDE_DIRS}
)

file(GLOB headers
  genericprototype.h
)

file(GLOB sources
  genericprototype.cpp
)

add_executable(GenericPrototype ${sources} ${headers})
target_link_libraries(GenericPrototype
  gamekeeperBackend
  gamekeeperClientMain
)

if(BUILD_USE_COTIRE)
  cotire(GenericPrototype)
endif()
