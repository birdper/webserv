
set(CMAKE_CXX_STANDARD 98)

set(MODULE_NAME cgi)

add_library(${MODULE_NAME}
        CGIHandler.cpp CGIHandler.hpp
        )

target_include_directories(${MODULE_NAME} PUBLIC
        ../exception
        ../model
        ../util
        )

include_directories(${MODULE_NAME} PUBLIC
        ../exception
        ../model
        ../util
        )
