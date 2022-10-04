
set(CMAKE_CXX_STANDARD 98)

set(MODULE_NAME interacts)

add_library(${MODULE_NAME}
        RequestParser.hpp RequestParser.cpp
        RequestHandler.hpp RequestHandler.cpp
        BaseHandler.cpp BaseHandler.hpp
        GetHandler.cpp GetHandler.hpp
        PostHandler.cpp PostHandler.hpp
        DeleteHandler.cpp DeleteHandler.hpp
        )


target_include_directories(${MODULE_NAME} PUBLIC ../util ../exception  ../model)

include_directories(${MODULE_NAME} PUBLIC ../util ../exception ../model)
