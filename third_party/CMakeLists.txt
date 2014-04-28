# disable warnings and stuff
add_compiler_flags(-w)

if(BUILD_TESTS)
  add_subdirectory(mongoose)
endif()
