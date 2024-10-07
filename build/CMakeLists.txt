cmake_minimum_required(VERSION 3.5)
project(WreathJSON)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../lib)

set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "--target=x86_64-w64-mingw32")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(WreathLibJSON_Static STATIC ../src/JSONToken.cpp ../src/JSONFile.cpp)
add_library(WreathLibJSON_Shared SHARED ../src/JSONToken.cpp ../src/JSONFile.cpp)
target_include_directories(WreathLibJSON_Static PRIVATE ../include)
target_include_directories(WreathLibJSON_Shared PRIVATE ../include)
