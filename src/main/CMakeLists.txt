cmake_minimum_required(VERSION 3.21)

set(CMAKE_CXX_STANDARD 98)

set(PROJECT_NAME main)

add_library(${PROJECT_NAME} SHARED
        exception
        util
        model
        config
        parser
        cgi
        interacts
        server
        )

add_executable(webserv
        main.cpp
        )

add_subdirectory(exception)
add_subdirectory(util)
add_subdirectory(model)
add_subdirectory(config)
add_subdirectory(parser)
add_subdirectory(cgi)
add_subdirectory(interacts)
add_subdirectory(server)

target_link_libraries(${PROJECT_NAME} PUBLIC
        exception
        util
        model
        config
        parser
        cgi
        interacts
        server
        )

target_include_directories(${PROJECT_NAME} PUBLIC
        exception
        util
        model
        repository
        config/model
        config/repository
        config/storage
        config/types
        parser
        cgi
        interacts
        server
        )

target_link_libraries(webserv PUBLIC
        main
        )
