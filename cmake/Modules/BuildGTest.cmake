if(MSVC11)
  set(ADD_CXX_FLAGS "/D_VARIADIC_MAX=10")
endif()

ExternalProject_Add(
  gtest
  URL "${GTEST_URL}"
  URL_MD5 ${GTEST_MD5}
  CMAKE_ARGS -DBUILD_SHARED_LIBS=ON -DCMAKE_CXX_FLAGS=${ADD_CXX_FLAGS} -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE} -Dgtest_force_shared_crt:BOOL=ON
  BUILD_IN_SOURCE 1
  INSTALL_COMMAND ""
)

ExternalProject_Get_Property(
  gtest
  source_dir
)

set(GTEST_INCLUDE_DIRS "${source_dir}/include")
set(GTEST_INSTALL_LIB "${source_dir}/${CMAKE_SHARED_LIBRARY_PREFIX}gtest${CMAKE_SHARED_LIBRARY_SUFFIX}")
if(WIN32)
  set(GTEST_LIBRARIES "${source_dir}/${CMAKE_IMPORT_LIBRARY_PREFIX}gtest${CMAKE_IMPORT_LIBRARY_SUFFIX}")
else()
  set(GTEST_LIBRARIES "${GTEST_INSTALL_LIB}")
endif()

set(GTEST_BOTH_LIBRARIES ${GTEST_LIBRARIES})

install_external_library(gtest ${GTEST_INSTALL_LIB})
