include(UseJava)

include_directories(
  ${JNI_INCLUDE_DIRS}
)

add_gamekeeper_swig_binding(java GameKeeperModelJava Model -package org.gamekeeper.bindings.model)

# <cmake-2.8.12 backward compatibility
set(CMAKE_JAVA_TARGET_OUTPUT_NAME GameKeeper)

add_jar(GameKeeperModelJavaJar
    "${SWIG_java_Model_MODULE_PATH}/Game.java"
    "${SWIG_java_Model_MODULE_PATH}/GameKeeperModelJava.java"
    "${SWIG_java_Model_MODULE_PATH}/GameKeeperModelJavaJNI.java"
  OUTPUT_NAME GameKeeperModelJava
)
get_property(target_loc TARGET GameKeeperModelJavaJar PROPERTY INSTALL_FILES)
add_custom_command(TARGET GameKeeperModelJavaJar POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "${target_loc}" "${SWIG_java_Model_LIBRARY_PATH}")
unset(target_loc)

add_dependencies(GameKeeperModelJavaJar ${SWIG_MODULE_GameKeeperModelJava_REAL_NAME})
