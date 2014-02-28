if(NOT MINGW)
  message(FATAL_ERROR "jsoncpp build is currently only supported with mingw")
else()
  ExternalProject_Add(
    jsoncpp
    URL ${JSONCPP_URL}
    URL_MD5 ${JSONCPP_MD5}
    UPDATE_COMMAND ""
    CONFIGURE_COMMAND ""
    BUILD_COMMAND /c/Python27/Python /c/Python27/Scripts/scons.py platform=mingw
    BUILD_IN_SOURCE 1
  )
  
  ExternalProject_Get_Property(
    jsoncpp
    source_dir
  )
  
  set(JSONCPP_LIBRARIES "${source_dir}/libs/mingw/libjson_mingw_libmt.a")
  set(JSONCPP_INCLUDE_DIRS "${source_dir}/include")
endif()
