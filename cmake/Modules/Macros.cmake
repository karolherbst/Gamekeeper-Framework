#######################################################################################################################
# some global macros/functions
#######################################################################################################################

# http://www.itk.org/Wiki/CMakeMacroParseArguments

MACRO(PARSE_ARGUMENTS prefix arg_names option_names)
  SET(DEFAULT_ARGS)
  FOREACH(arg_name ${arg_names})    
    SET(${prefix}_${arg_name})
  ENDFOREACH(arg_name)
  FOREACH(option ${option_names})
    SET(${prefix}_${option} FALSE)
  ENDFOREACH(option)

  SET(current_arg_name DEFAULT_ARGS)
  SET(current_arg_list)
  FOREACH(arg ${ARGN})            
    SET(larg_names ${arg_names})    
    LIST(FIND larg_names "${arg}" is_arg_name)                   
    IF (is_arg_name GREATER -1)
      SET(${prefix}_${current_arg_name} ${current_arg_list})
      SET(current_arg_name ${arg})
      SET(current_arg_list)
    ELSE (is_arg_name GREATER -1)
      SET(loption_names ${option_names})    
      LIST(FIND loption_names "${arg}" is_option)            
      IF (is_option GREATER -1)
         SET(${prefix}_${arg} TRUE)
      ELSE (is_option GREATER -1)
         SET(current_arg_list ${current_arg_list} ${arg})
      ENDIF (is_option GREATER -1)
    ENDIF (is_arg_name GREATER -1)
  ENDFOREACH(arg)
  SET(${prefix}_${current_arg_name} ${current_arg_list})
ENDMACRO(PARSE_ARGUMENTS)

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

function(add_copy_target_step target)
  get_target_property(target_loc ${target} LOCATION)
  add_copy_target_file_step(${target} ${target_loc})
endfunction()

function(install_external_library target system_wide)
  foreach(file ${ARGN})
    if(${system_wide})
      install(CODE "get_filename_component(resolved_file \"${file}\" REALPATH)\n get_filename_component(file_name \"${file}\" NAME)\n file(INSTALL DESTINATION ${LIB_INSTALL_DIR} TYPE FILE RENAME \${file_name} FILES \"\${resolved_file}\" )")
    endif()
    add_copy_target_file_step(${target} "${file}")
  endforeach()
endfunction()

function(install_library target)
  # dlls are runtime targets, import libs are archive, but we don't need them
  install(TARGETS "${target}"
          RUNTIME DESTINATION "${LIB_INSTALL_DIR}"
          LIBRARY DESTINATION "${LIB_INSTALL_DIR}")
  add_copy_target_step(${target})
endfunction()

#######################################################################################################################
# compiler stuff
#######################################################################################################################

macro(add_compiler_flags)
  set(flags_list "")
  parse_arguments(ARG "" "C;CXX;DEBUG;RELEASE" ${ARGN})

  if(NOT ARG_DEBUG AND NOT ARG_RELEASE)
    foreach(flag ${ARG_DEFAULT_ARGS})
      set(flags_list "${flags_list} ${flag}")
    endforeach()
    if(NOT ARG_C AND NOT ARG_CXX OR ARG_C)
      set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${flags_list}")
    endif()
    if(NOT ARG_C AND NOT ARG_CXX OR ARG_CXX)
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${flags_list}")
    endif()
  elseif(ARG_DEBUG)
    foreach(flag ${ARG_DEFAULT_ARGS})
      set(flags_list "${flags_list} ${flag}")
    endforeach()
    if(NOT ARG_C AND NOT ARG_CXX OR ARG_C)
      set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} ${flags_list}")
    endif()
    if(NOT ARG_C AND NOT ARG_CXX OR ARG_CXX)
      set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${flags_list}")
    endif()
  elseif(ARG_RELEASE)
    foreach(flag ${ARG_DEFAULT_ARGS})
      set(flags_list "${flags_list} ${flag}")
    endforeach()
    if(NOT ARG_C AND NOT ARG_CXX OR ARG_C)
      set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} ${flags_list}")
      set(CMAKE_C_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} ${flags_list}")
    endif()
    if(NOT ARG_C AND NOT ARG_CXX OR ARG_CXX)
      set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${flags_list}")
      set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} ${flags_list}")
    endif()
  endif()
endmacro()

macro(add_linker_flags)
  foreach(flag ${ARGN})
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${flag}")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${flag}")
  endforeach()
endmacro()
