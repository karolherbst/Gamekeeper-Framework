if(EXISTS ${CMAKE_COMPILER_PATH}/${CMAKE_CXX_COMPILER_ID}.cmake)
  include(${CMAKE_COMPILER_PATH}/${CMAKE_CXX_COMPILER_ID}.cmake)
else()
  message("${CMAKE_CXX_COMPILER_ID} is not a supported compiler! We will try to use a gcc config.")
    include(${CMAKE_COMPILER_PATH}/GccCompatible.cmake)
endif()