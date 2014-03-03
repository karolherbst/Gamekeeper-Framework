find_package(Jsoncpp ${REQUIRED_ON_LINUX})
if(NOT JSONCPP_FOUND)
  set_download_url(JSONCPP http://sourceforge.net/projects/jsoncpp/files/jsoncpp/0.6.0-rc2/jsoncpp-src-0.6.0-rc2.tar.gz/download 363e2f4cbd3aeb63bf4e571f377400fb)
  include(BuildJSONCpp)
endif()

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${GAMEKEEPER_INCLUDE_DIR}
  ${Boost_INCLUDE_DIRS}
  ${HYPODERMIC_INCLUDE_DIRS}
  ${JSONCPP_INCLUDE_DIRS}
)

file(GLOB headers
  gameolithprototype.h
)

file(GLOB sources
  gameolithprototype.cpp
)

add_executable(GameolithPrototype ${sources} ${headers})
target_link_libraries(GameolithPrototype
  gamelibClientMain
  gamekeeperModelBindings
  ${JSONCPP_LIBRARIES}
)
