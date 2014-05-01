include(${CMAKE_COMPILER_PATH}/GccCompatible.cmake)
message(STATUS "using GCC")
set(GCC TRUE)

add_compiler_flags(CXX
  -Wno-conversion-null
)
