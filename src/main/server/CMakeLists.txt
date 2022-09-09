
set(CMAKE_CXX_STANDARD 98)

set(PROJECT_NAME server)

add_library(${PROJECT_NAME}
        Constants.hpp
        Server.hpp Server.cpp
        Socket.hpp Socket.cpp
        ClientRepository.hpp ClientRepository.cpp
        )

target_include_directories(${PROJECT_NAME} PRIVATE
        ../model
        ../repository
        ../config/repository
        ../interacts
        )
