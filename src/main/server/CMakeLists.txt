
set(CMAKE_CXX_STANDARD 98)

set(NAME_MODULE "server")

add_library(${NAME_MODULE}
        HttpServer.cpp HttpServer.hpp
        )

include_directories(${NAME_MODULE} PRIVATE model)