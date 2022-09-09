
set(CMAKE_CXX_STANDARD 98)


set(MODULE_NAME parser)

add_library(${MODULE_NAME}
        Token.cpp Token.hpp
        Tokenizer.cpp Tokenizer.hpp
        ConfigParser.cpp ConfigParser.hpp
        )

target_include_directories(${MODULE_NAME} PUBLIC ../model)
target_include_directories(${MODULE_NAME} PUBLIC ../config/model)
target_include_directories(${MODULE_NAME} PUBLIC ../config/storage)
target_include_directories(${MODULE_NAME} PUBLIC ../config/types)
#target_link_directories(parser PUBLIC ../config)
