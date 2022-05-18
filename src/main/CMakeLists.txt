cmake_minimum_required(VERSION 3.21)

set(CMAKE_CXX_STANDARD 98)

set(PROJECT_NAME main)

add_library(${PROJECT_NAME} SHARED
        domain
        data
        parser
        interacts
        server
        )

add_executable(webserv
        main.cpp
        )

add_subdirectory(domain)
add_subdirectory(data)
add_subdirectory(parser)
add_subdirectory(interacts)
add_subdirectory(server)

target_link_libraries(${PROJECT_NAME} PUBLIC
        domain
        data
        parser
        interacts
        server
        )

target_include_directories(${PROJECT_NAME} PUBLIC
        domain/model
        domain/repository
        data/model
        data/repository
        data/storage
        data/types
        parser
        interacts
        server
        )

target_link_libraries(webserv PUBLIC
        main
        )
