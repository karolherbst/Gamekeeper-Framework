# we compile as a shared library and with hidden symbols by default
add_compiler_flags(-fvisibility=hidden)
if(NOT MINGW)
  add_compiler_flags(-fPIC)
endif()

# we use the new c++11 standard for C++
# we will use the std=c++0x flag until we run into any incompatibilty.
# std=c++11 requires gcc-4.7+
add_compiler_flags(CXX -std=c++0x)

# having as-needed enabled is good practise
add_linker_flags(-Wl,--as-needed)
