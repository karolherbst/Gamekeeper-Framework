include(${CMAKE_COMPILER_PATH}/GccCompatible.cmake)

# add gcc compatibility flag on Linux systems
if(LINUX)
  add_compiler_flags(-gcc)
endif()

message(STATUS "using ICC")
set(ICC TRUE)
