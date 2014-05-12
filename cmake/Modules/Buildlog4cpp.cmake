if(WIN32 AND NOT MINGW)
  message(FATAL_ERROR "windows build is currently only supported with mingw")
else()
  if(MINGW)
    if(DEFINED CMAKE_TOOLCHAIN_FILE)
      set(CROSSCOMPILE_OPTIONS -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE})
    endif()

    set(LOG4CPP_INSTALL_DIR ${CMAKE_EXTERNAL_BINARY_DIR}/log4cpp)
    ExternalProject_Add(
      log4cpp
      URL ${LOG4CPP_URL}
      URL_MD5 ${LOG4CPP_MD5}
      UPDATE_COMMAND ""
      PATCH_COMMAND ${PATCH_EXECUTABLE} -p1 -i ${CMAKE_PATCH_DIR}/log4cpp-mingw.patch
      CMAKE_ARGS ${CROSSCOMPILE_OPTIONS} -DCMAKE_INSTALL_PREFIX=${LOG4CPP_INSTALL_DIR} -DCMAKE_CXX_FLAGS=${ADD_CXX_FLAGS} -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
    )
    set(LOG4CPP_LIBRARIES "${LOG4CPP_INSTALL_DIR}/lib/liblog4cpp.dll.a")
    set(LOG4CPP_INCLUDE_DIR "${LOG4CPP_INSTALL_DIR}/include")
  else()
    message(FATAL_ERROR "building log4cpp isn't supported on plattform except mingw")
  endif()
endif()
