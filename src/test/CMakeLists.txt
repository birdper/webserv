cmake_minimum_required(VERSION 3.21)
set(CMAKE_CXX_STANDARD 98)

set(PROJECT_NAME unit_test)

set(SOURCES
        config/storage/TestConfigStorage.cpp
        interacts/TestRequestParser.cpp
        parser/TestConfigParser.cpp
        acutest.h
        )

add_executable(${PROJECT_NAME}
        ../main
        ${SOURCES}
        )

target_link_libraries(${PROJECT_NAME} PUBLIC
        main
        )

target_include_directories(${PROJECT_NAME} PUBLIC .)

#add_test(
#        PROJECT_NAME ${PROJECT_NAME}
#        COMMAND ${PROJECT_NAME}
#)

