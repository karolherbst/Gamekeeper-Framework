include(UseJava)

include_directories(
  ${JNI_INCLUDE_DIRS}
)

add_gamelib_swig_binding(java GameLibJava -package org.gamelib.bindings)

# <cmake-2.8.12 backward compatibility
set(CMAKE_JAVA_TARGET_OUTPUT_NAME GameLib)

add_jar(GameLibJavaJar
    "${SWIG_java_MODULE_PATH}/Game.java"
    "${SWIG_java_MODULE_PATH}/GameLibJava.java"
    "${SWIG_java_MODULE_PATH}/GameLibJavaJNI.java"
  OUTPUT_NAME GameLib
)
get_property(target_loc TARGET GameLibJavaJar PROPERTY INSTALL_FILES)
add_custom_command(TARGET GameLibJavaJar POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "${target_loc}" "${CMAKE_BINDINGS_OUT_DIR}/java/")
unset(target_loc)

