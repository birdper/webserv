cmake_minimum_required(VERSION 3.21)
project(webserv)

set(CMAKE_CXX_STANDARD 98)

set(PATH "src/main")
set(PROJECT_NAME webserv)

add_subdirectory(src/main)

#enable_testing()
add_subdirectory(src/test)

#[[

add_executable(${PROJECT_NAME}
        ${PATH}/main.cpp
        ${PATH}/domain
        ${PATH}/data
        ${PATH}/parser
        ${PATH}/interacts
        ${PATH}/server
        )


add_subdirectory(${PATH}/domain)
add_subdirectory(${PATH}/data)
add_subdirectory(${PATH}/parser)
add_subdirectory(${PATH}/interacts)
add_subdirectory(${PATH}/server)

target_link_libraries(${PROJECT_NAME} PUBLIC domain)
target_include_directories(${PROJECT_NAME} PUBLIC ${PATH}/domain)

target_link_libraries(${PROJECT_NAME} PUBLIC data)
target_include_directories(${PROJECT_NAME} PUBLIC ${PATH}/data)

target_link_libraries(${PROJECT_NAME} PUBLIC parser)
target_include_directories(${PROJECT_NAME} PUBLIC ${PATH}/parser)

target_link_libraries(${PROJECT_NAME} PUBLIC interacts)
target_include_directories(${PROJECT_NAME} PUBLIC ${PATH}/interacts)

target_link_libraries(${PROJECT_NAME} PUBLIC server)
target_include_directories(${PROJECT_NAME} PUBLIC ${PATH}/server)

]]

#add_executable(${PROJECT_NAME}
#        src/main/main.cpp
#        src/main
#        )


#add_executable(${PROJECT_NAME}
#        src/main/main.cpp
##        main
#        )

#target_link_libraries(${PROJECT_NAME} src/main)





#add_executable(test
#        unit_test)
#
#target_link_libraries(${PROJECT_NAME} PUBLIC
#        main
#        unit_test)


#target_link_libraries(${PROJECT_NAME} PUBLIC
#        domain
#        data
#        parser
#        interacts
#        server
#        )

#target_include_directories(${PROJECT_NAME} PUBLIC
#        ${PATH}/domain
#        ${PATH}/data
#        ${PATH}/parser
#        ${PATH}/interacts
#        ${PATH}/server
#        )


#target_include_directories(${PROJECT_NAME} PUBLIC src/main/domain src/main/data)



#enable_testing ()
#add_test (PROJECT_NAME MyTest COMMAND Test)
