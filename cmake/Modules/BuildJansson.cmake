if(WIN32 AND NOT MINGW)
  message(FATAL_ERROR "windows build is currently only supported with mingw")
else()
  if(MINGW)
    if(DEFINED CMAKE_TOOLCHAIN_FILE)
      set(CROSSCOMPILE_OPTIONS -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE})
    endif()

    set(JANSSON_INSTALL_DIR ${CMAKE_EXTERNAL_BINARY_DIR}/jansson)
    ExternalProject_Add(
      jansson
      URL ${JANSSON_URL}
      URL_MD5 ${JANSSON_MD5}
      UPDATE_COMMAND ""
      PATCH_COMMAND ${PATCH_EXECUTABLE} -p1 -i ${CMAKE_PATCH_DIR}/jansson-fix-mingw.patch
      CMAKE_ARGS ${CROSSCOMPILE_OPTIONS} -DCMAKE_INSTALL_PREFIX=${JANSSON_INSTALL_DIR} -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
                 -DBUILD_SHARED_LIBS=ON -DBUILD_DOCS=OFF -DWITHOUT_TESTS=ON
    )
    set(JANSSON_LIBRARIES "${JANSSON_INSTALL_DIR}/lib/libjansson.dll.a")
    set(JANSSON_INCLUDE_DIR "${JANSSON_INSTALL_DIR}/include")
  else()
    message(FATAL_ERROR "building jansson isn't supported on plattform except mingw")
  endif()
endif()
