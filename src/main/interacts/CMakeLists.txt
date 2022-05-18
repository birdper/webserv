
set(CMAKE_CXX_STANDARD 98)

add_library(interacts
        RequestHandler.hpp RequestHandler.cpp
        RequestParser.hpp
        RequestParser.cpp
        )

target_include_directories(interacts PUBLIC ../domain/model)