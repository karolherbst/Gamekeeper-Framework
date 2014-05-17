include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${JANSSON_INCLUDE_DIR}
)

set(JANSSON_PATH_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR} CACHE INTERNAL "path to jansson_path include dir")

file(GLOB headers
  jansson_path.h
)

file(GLOB sources
  jansson_path.c
)

add_library(jansson_path STATIC ${sources} ${headers})
target_link_libraries(jansson_path
  ${JANSSON_LIBRARIES}
)
