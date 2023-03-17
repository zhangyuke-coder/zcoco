cmake_minimum_required(VERSION 3.0)

project(zcoco CXX C)

set(CXX_FLAGS
 -O2
 -g
 -pg
 -Wall
 -Wextra
 -Werror
 -std=c++11
 -pthread
)

string(REPLACE ";" " " CMAKE_CXX_FLAGS "${CXX_FLAGS}")
include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${GMOCK_INCLUDE_DIR})

find_package(Threads REQUIRED)
find_package(Protobuf REQUIRED)
if(PROTOBUF_FOUND)
   message(STATUS "protobuf library found")
else()
    message(FATAL_ERROR "protobuf library is needed but cant be found")
endif()
find_package(ZLIB REQUIRED)
if (ZLIB_FOUND)
    include_directories(${ZLIB_INCLUDE_DIRS})
endif()

# --[ Output directory
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
add_subdirectory(test)
add_subdirectory(src)
add_subdirectory(example)