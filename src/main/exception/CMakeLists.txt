
set(CMAKE_CXX_STANDARD 98)

set(MODULE_NAME exception)

add_library(${MODULE_NAME}
        ErrorPagePathNotFoundException.hpp ErrorPagePathNotFoundException.cpp
        FileNotFoundException.hpp FileNotFoundException.cpp
        )

#target_include_directories(${MODULE_NAME} PUBLIC ./)

