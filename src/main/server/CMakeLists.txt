
set(CMAKE_CXX_STANDARD 98)

set(PROJECT_NAME server)

add_library(${PROJECT_NAME}
        Constants.hpp
        Server.cpp Server.hpp
        Socket.cpp Socket.hpp
        )

target_include_directories(${PROJECT_NAME} PRIVATE
        ../domain/model
        ../domain/repository
        ../data/repository
        ../interacts
        )
