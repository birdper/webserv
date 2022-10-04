cmake_minimum_required(VERSION 3.21)

project(webserv)

set(CMAKE_CXX_STANDARD 98)

set(PATH "src/main")
set(PROJECT_NAME webserv)

add_subdirectory(src/main)

add_subdirectory(src/test)
