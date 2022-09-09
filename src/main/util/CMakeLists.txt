
set(CMAKE_CXX_STANDARD 98)

set(PROJECT_NAME util)

add_library(${PROJECT_NAME}
        FileReader.hpp FileReader.cpp
        Utils.hpp Utils.cpp
        )
