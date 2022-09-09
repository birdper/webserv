
set(CMAKE_CXX_STANDARD 98)

set(MODULE_NAME model)

add_library(${MODULE_NAME}
#        ../repository/ConfigRepository.hpp
        Client.hpp Client.cpp
        Parameters.hpp
        Config.hpp Config.cpp
        HttpMessage.cpp HttpMessage.hpp
        HttpMethod.hpp
        Request.hpp Request.cpp
        Response.hpp Response.cpp
        )

#include_directories(${MODULE_NAME} PRIVATE ../repository .)

target_include_directories(model PUBLIC
        ../exception
        )

#include_directories(${MODULE_NAME} PUBLIC
#        ../exception)

