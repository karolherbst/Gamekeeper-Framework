include_directories(
  repo
)

set(MONGOOSE_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/repo CACHE INTERNAL "path to mongoose include dir")

# disable some functionality we don't need
add_definitions(-DMONGOOSE_NO_FILESYSTEM -DMONGOOSE_NO_WEBSOCKET)

file(GLOB headers
  repo/mongoose.h
)

file(GLOB sources
  repo/mongoose.c
)

add_library(mongoose ${sources} ${headers})
target_link_libraries(mongoose
  ${CMAKE_DL_LIBS}
)
