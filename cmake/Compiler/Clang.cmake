include(${CMAKE_COMPILER_PATH}/GccCompatible.cmake)
message(STATUS "using clang")
set(CLANG TRUE)

# disable some clang only warnings
add_compiler_flags(
  -Wno-c++98-compat
  -Wno-c++98-compat-pedantic
  -Wno-disabled-macro-expansion
  -Wno-documentation
  -Wno-exit-time-destructors
  -Wno-global-constructors
  -Wno-weak-vtables
)

# some hacks

# clang has variadic macro support in c++11 mode, so just enable it for boost, because boost thinks it has not
add_definitions(-DBOOST_PP_VARIADICS)
