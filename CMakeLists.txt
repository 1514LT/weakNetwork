cmake_minimum_required(VERSION 3.20)
project("weakNet")

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  ${CMAKE_CURRENT_SOURCE_DIR}/data
)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_EXTENSIONS OFF)

message(STATUS "Configuring Base Lib...")
add_library(
  Base_lib
  SHARED
  
  ${CMAKE_CURRENT_SOURCE_DIR}/data/data.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/handler/handler.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/dataProcess.cpp

  ${CMAKE_CURRENT_SOURCE_DIR}/discover/discover.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/publisher/publisher.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/subscriber/subscriber.cpp
)
# 依赖子目录编译的动态库
add_dependencies(Base_lib data_shared)

if(DEFINED ARM64)
message(STATUS "ARM64 is defined")
# target_link_libraries(
#   Base_lib
#   ${CMAKE_CURRENT_SOURCE_DIR}/lib_arm64/
# )
else()
target_link_libraries(
  Base_lib
  ${CMAKE_CURRENT_SOURCE_DIR}/lib_x86/liblcm.so
  ${CMAKE_CURRENT_SOURCE_DIR}/lib_x86/libsnappy.so
  ${CMAKE_CURRENT_SOURCE_DIR}/lib_x86/libjsoncpp.so
  ${CMAKE_CURRENT_SOURCE_DIR}/lib_x86/libdata_shared.so
  ${CMAKE_CURRENT_SOURCE_DIR}/lib_x86/libz.so
  ${CMAKE_CURRENT_SOURCE_DIR}/lib_x86/libssl.so
  ${CMAKE_CURRENT_SOURCE_DIR}/lib_x86/libcrypto.so
)
endif()


foreach(_target
publisher
subscriber
discover
test
)
add_executable(
  ${_target} ${CMAKE_CURRENT_SOURCE_DIR}/${_target}/${_target}_main.cpp
)
target_link_libraries(
  ${_target}
  Base_lib
)

endforeach()
