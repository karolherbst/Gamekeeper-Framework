#######################################################################################################################
# some global macros/functions
#######################################################################################################################

#######################################################################################################################
# Downloads and stuff
#######################################################################################################################

macro(set_download_url name url md5)
  set(${name}_URL_DEFAULT "${url}")
  set(${name}_URL ${${name}_URL_DEFAULT} CACHE STRING "URL to get ${name} archive")
  if(${name}_URL STREQUAL ${name}_URL_DEFAULT)
    set(${name}_MD5 ${md5} CACHE STRING "Md5 for ${name} archive")
  else()
    unset(${name}_MD5)
  endif()
endmacro()

#######################################################################################################################
# custom install commands
#######################################################################################################################

function(add_copy_target_file_step target target_loc)
  get_filename_component(target_name "${target_loc}" NAME)

  if(${target_name} MATCHES ".*\\.exe")
    if(NOT IS_ABSOLUTE ${BINDIR})
      add_custom_command(TARGET ${target} POST_BUILD COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_BINARY_DIR}/${BINDIR}")
      add_custom_command(TARGET ${target} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "${target_loc}" "${CMAKE_BINARY_DIR}/${BINDIR}/${target_name}")
    endif()
  elseif(${target_name} MATCHES ".*\\${CMAKE_SHARED_LIBRARY_SUFFIX}" OR ${target_name} MATCHES ".*\\.sh")
    if(NOT IS_ABSOLUTE ${RUNTIME_LIBDIR})
      add_custom_command(TARGET ${target} POST_BUILD COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_BINARY_DIR}/${RUNTIME_LIBDIR}")
      add_custom_command(TARGET ${target} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "${target_loc}" "${CMAKE_BINARY_DIR}/${RUNTIME_LIBDIR}/${target_name}")
    endif()
  elseif(${target_name} MATCHES ".*\\${CMAKE_STATIC_LIBRARY_SUFFIX}")
    # this is a placeholder for API and import libs, also the static preloader for games
  else()
    # handle UNIX executeables here
    # get the extension first
    get_filename_component(target_ext ${target_name} EXT)
    if("${target_ext}" STREQUAL "")
      add_custom_command(TARGET ${target} POST_BUILD COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_BINARY_DIR}/${RUNTIME_LIBDIR}")
      add_custom_command(TARGET ${target} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "${target_loc}" "${CMAKE_BINARY_DIR}/${RUNTIME_LIBDIR}/${target_name}")
    endif()
  endif()
endfunction()

function(install_external_library target)
  foreach(file ${ARGN})
    install(CODE "get_filename_component(resolved_file \"${file}\" REALPATH)\n get_filename_component(file_name \"${file}\" NAME)\n file(INSTALL DESTINATION ${LIB_INSTALL_DIR} TYPE FILE RENAME \${file_name} FILES \"\${resolved_file}\" )")
    add_copy_target_file_step(${target} "${file}")
  endforeach()
endfunction()
