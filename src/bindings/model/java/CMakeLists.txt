include(UseJava)

include_directories(
  ${JNI_INCLUDE_DIRS}
)

add_gamelib_swig_binding(java GameLibModelJava Model -package org.gamelib.bindings.model)

# <cmake-2.8.12 backward compatibility
set(CMAKE_JAVA_TARGET_OUTPUT_NAME GameLib)

add_jar(GameLibModelJavaJar
    "${SWIG_java_Model_MODULE_PATH}/Game.java"
    "${SWIG_java_Model_MODULE_PATH}/GameLibModelJava.java"
    "${SWIG_java_Model_MODULE_PATH}/GameLibModelJavaJNI.java"
  OUTPUT_NAME GameLibModel
)
get_property(target_loc TARGET GameLibModelJavaJar PROPERTY INSTALL_FILES)
add_custom_command(TARGET GameLibModelJavaJar POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "${target_loc}" "${SWIG_java_Model_LIBRARY_PATH}")
unset(target_loc)

