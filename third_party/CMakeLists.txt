# disable warnings and stuff
add_compiler_flags(-w)

if(NOT PUGIXML_FOUND)
  add_subdirectory(pugixml)
endif()

if(BUILD_TESTS)
  add_subdirectory(mongoose)
endif()

add_subdirectory(jansson_path)
