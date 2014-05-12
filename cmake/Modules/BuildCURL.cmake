if(WIN32 AND NOT MINGW)
  ExternalProject_Add(
    curl
    URL ${CURL_URL}
    URL_MD5 ${CURL_MD5}
    UPDATE_COMMAND ""
    CONFIGURE_COMMAND ""
    BUILD_IN_SOURCE 1
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
  )
  ExternalProject_Add_Step(
    curl
    custom_build
    DEPENDEES configure
    DEPENDERS build
    COMMAND ${NMAKE_EXECUTABLE} /f Makefile.vc MODE=dll DEBUG=${CURL_DEBUG} GEN_PDB=no ENABLE_SSPI=yes ENABLE_IPV6=no ENABLE_IDN=no ENABLE_WINSSL=yes MACHINE=x86 ENABLE_SPNEGO=no
    WORKING_DIRECTORY <SOURCE_DIR>/winbuild
  )
  
  ExternalProject_Get_Property(
    curl
    source_dir
  )
  
  set(CURL_INSTALL_DIR ${source_dir}/builds/libcurl-vc-x86-release-dll-sspi-winssl)
else()
  if(MINGW)
    set(CURL_SSL_SWITCH --with-winssl)
  else()
    find_package(OpenSSL REQUIRED)
    set(CURL_SSL_SWITCH --with-ssl)
  endif()
  set(CURL_INSTALL_DIR ${CMAKE_EXTERNAL_BINARY_DIR}/curl)

  if(DEFINED CMAKE_TOOLCHAIN_FILE)
    set(CROSSCOMPILE_OPTIONS -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE})
  endif()
  ExternalProject_Add(
    curl
    URL ${CURL_URL}
    URL_MD5 ${CURL_MD5}
    UPDATE_COMMAND ""
    CMAKE_ARGS ${CROSSCOMPILE_OPTIONS} -DBUILD_SHARED_LIBS=ON -DCMAKE_CXX_FLAGS=${ADD_CXX_FLAGS} -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE} -DCMAKE_INSTALL_PREFIX=${CURL_INSTALL_DIR}
      -DBUILD_CURL_EXE=OFF -DBUILD_CURL_TESTS=OFF -DCURL_DISABLE_LDAP=ON -DCURL_DISABLE_TELNET=ON -DCURL_DISABLE_DICT=ON -DCURL_DISABLE_FILE=ON -DCURL_DISABLE_TFTP=ON -DCURL_DISABLE_LDAPS=ON
  )
endif()

set(CURL_BIN_DIRS ${CURL_INSTALL_DIR}/bin)
set(CURL_LIBRARY_DIR ${CURL_INSTALL_DIR}/lib)
set(CURL_INCLUDE_DIRS ${CURL_INSTALL_DIR}/include)

if(WIN32 AND NOT MINGW)
  list(APPEND CURL_LIBRARIES "${CURL_LIBRARY_DIR}/libcurl.lib")
  install_external_library(curl "${CURL_BIN_DIRS}/libcurl.dll")
else()
  list(APPEND CURL_LIBRARIES "${OPENSSL_LIBRARIES}")
  if(MINGW)
    list(APPEND CURL_LIBRARIES "${CURL_LIBRARY_DIR}/libcurl_imp.lib")
    list(APPEND CURL_LIBRARIES "ws2_32")
  else()
    list(APPEND CURL_LIBRARIES "${CURL_LIBRARY_DIR}/libcurl.a")
    list(APPEND CURL_LIBRARIES "rt")
  endif()
endif()
