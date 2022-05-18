
set(CMAKE_CXX_STANDARD 98)


add_library(parser
        Token.cpp Token.hpp
        Tokenizer.cpp Tokenizer.hpp
        ConfigParser.cpp ConfigParser.hpp
        SettingsCollector.hpp
        )

target_include_directories(parser PUBLIC ../domain/model)
target_include_directories(parser PUBLIC ../data/model)
target_include_directories(parser PUBLIC ../data/storage)
target_include_directories(parser PUBLIC ../data/types)
#target_link_directories(parser PUBLIC ../data)
