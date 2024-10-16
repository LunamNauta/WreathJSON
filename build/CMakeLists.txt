cmake_minimum_required(VERSION 3.5)
project(wreath-json)

set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "--target=x86_64-w64-mingw32")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_library(wreathjson_st STATIC ../src/wreath/json/token.cpp ../src/wreath/json/file.cpp)
add_library(wreathjson_sh SHARED ../src/wreath/json/token.cpp ../src/wreath/json/file.cpp)
target_include_directories(wreathjson_st PRIVATE ../include)
target_include_directories(wreathjson_sh PRIVATE ../include)
