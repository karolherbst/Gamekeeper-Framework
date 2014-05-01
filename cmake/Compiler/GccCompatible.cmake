# we compile as a shared library and with hidden symbols by default
add_compiler_flags(-fvisibility=hidden)
if(NOT MINGW)
  add_compiler_flags(-fPIC)
endif()

# we use the new c++11 standard for C++
# we will use the std=c++0x flag until we run into any incompatibilty.
# std=c++11 requires gcc-4.7+
if(MINGW)
  add_compiler_flags(CXX -std=gnu++0x)
else()
  add_compiler_flags(CXX -std=c++0x)
endif()

# having as-needed enabled is good practise
add_linker_flags(-Wl,--as-needed)

# disable some warnings
add_compiler_flags(
  -Wno-padded
  -Wno-sign-conversion
  -Wno-switch-enum
)

# enable warnings we want to enforce
add_compiler_flags(-Werror
  -Wswitch
)
