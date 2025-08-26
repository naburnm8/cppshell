cmake_minimum_required(VERSION 3.30)
project(cppshell)

set(CMAKE_CXX_STANDARD 20)

add_library(command STATIC src/command.cpp)
target_include_directories(command PUBLIC include)

add_library(fs_manipulation STATIC src/fs_manipulation.cpp)
target_include_directories(fs_manipulation PUBLIC include)

add_library(redirect STATIC src/redirect.cpp)
target_include_directories(redirect PUBLIC include)

add_library(shell STATIC src/shell.cpp)
target_include_directories(shell PUBLIC include)

target_link_libraries(shell command fs_manipulation redirect)
add_executable(cppshell src/main.cpp)

target_link_libraries(cppshell shell)
