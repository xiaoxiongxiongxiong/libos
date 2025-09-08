# CMakeList.txt : CMake project for libos, include source and define
# project specific logic here.
#
cmake_minimum_required(VERSION 3.8)
message("CMake version: " ${CMAKE_VERSION})

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

project("libos" VERSION 1.0.0)

add_subdirectory(src obj/src)

option(OS_BUILD_WITH_EXAMPLES "build with examples" TRUE)
if (OS_BUILD_WITH_EXAMPLES)
    message("libos build with examples ...")
	add_subdirectory(examples obj/examples)
endif()
