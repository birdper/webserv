cmake_minimum_required(VERSION 3.21)
project(webserv)

set(CMAKE_CXX_STANDARD 98)

set(PATH "src/main")
set(NAME "webserv")

add_subdirectory(${PATH}/domain)
add_subdirectory(${PATH}/data)
add_subdirectory(${PATH}/parser)
add_subdirectory(${PATH}/server)

add_executable(${NAME}
        main.cpp
        )

target_link_libraries(${NAME} PUBLIC domain)
target_include_directories(${NAME} PUBLIC ${PATH}/domain)

target_link_libraries(${NAME} PUBLIC data)
target_include_directories(${NAME} PUBLIC ${PATH}/data)

target_link_libraries(${NAME} PUBLIC parser)
target_include_directories(${NAME} PUBLIC ${PATH}/parser)

target_link_libraries(${NAME} PUBLIC server)
target_include_directories(${NAME} PUBLIC ${PATH}/server)