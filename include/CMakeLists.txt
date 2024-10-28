cmake_minimum_required(VERSION 3.12)
project(data_library)
set(CMAKE_CXX_STANDARD 11)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

if(DEFINED ARM64)

  message(STATUS "ARM64 is defined")
  set(LIB ${CMAKE_CURRENT_SOURCE_DIR}/../lib_arm64)

  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${LIB})
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${LIB})

  add_library(
    data_shared
    SHARED
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/dataProcess.cpp
  )

  add_library(
    data_static
    STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/dataProcess.cpp
  )
  target_link_libraries(
    data_shared
    PRIVATE ${LIB}/libjsoncpp.so
  )
else()

  message(STATUS "default config")
  set(LIB ${CMAKE_CURRENT_SOURCE_DIR}/../lib_x86)
  message(STATUS "LIB:${LIB}")
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${LIB})
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${LIB})

  add_library(
    data_shared
    SHARED
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/dataProcess.cpp
  )

  add_library(
    data_static
    STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/dataProcess.cpp
  )

  target_link_libraries(
    data_shared
    PRIVATE ${LIB}/libjsoncpp.so
  )
endif()