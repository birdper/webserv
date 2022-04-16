
set(CMAKE_CXX_STANDARD 98)

set(NAME_MODULE "server")

add_library(${NAME_MODULE}
        HttpServer.cpp HttpServer.hpp
        SocketKqueue.cpp SocketKqueue.hpp
        Constants.hpp
        Server.cpp Server.hpp
        Socket.cpp Socket.hpp
        )

#include_directories(${NAME_MODULE} PRIVATE ../domain/model)
target_include_directories(${NAME_MODULE} PRIVATE ../domain/model)
