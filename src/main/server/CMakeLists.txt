
set(CMAKE_CXX_STANDARD 98)

set(NAME_MODULE "server")

add_library(${NAME_MODULE}
        Constants.hpp
        Server.cpp Server.hpp
        Socket.cpp Socket.hpp
        )

target_include_directories(${NAME_MODULE} PRIVATE ../domain/model)
target_include_directories(${NAME_MODULE} PRIVATE ../data/repository)
target_include_directories(${NAME_MODULE} PRIVATE ../interacts)
