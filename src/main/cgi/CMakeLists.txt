
set(CMAKE_CXX_STANDARD 98)

set(MODULE_NAME cgi)

add_library(${MODULE_NAME}
        CGI.cpp CGI.hpp
        )

target_include_directories(${MODULE_NAME} PUBLIC
        ../exception
        ../model
        ../util
        )

#include_directories(${MODULE_NAME} PUBLIC
#        ../exception
#        ../model
#        ../util
#        )
