# disable warnings and stuffm because we can't control the generated source code
add_compiler_flags(-w)
if(CLANG)
  add_compiler_flags(-Wno-c++11-narrowing)
endif()

add_subdirectory(model)
