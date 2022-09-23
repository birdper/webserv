
set(CMAKE_CXX_STANDARD 98)

set(MODULE_NAME util)

add_library(${MODULE_NAME}
        FileReader.hpp FileReader.cpp
        Utils.hpp Utils.cpp
        MimeTypesRepo.cpp MimeTypesRepo.hpp
        usings.hpp)

target_include_directories(${MODULE_NAME} PUBLIC
        ../exception
        )

#include_directories(${MODULE_NAME} PUBLIC
#        ../exception
#        )
