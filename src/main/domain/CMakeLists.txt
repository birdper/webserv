
set(CMAKE_CXX_STANDARD 98)

add_library(domain
        repository/SettingsRepository.hpp
        model/Config.hpp model/Config.cpp
        model/Parameters.hpp
        model/HttpMethod.hpp
        model/RequestParser.hpp
        model/RequestHandler.hpp
        model/WebClient.hpp model/WebClient.cpp
        model/Response.hpp)

include_directories(domain PRIVATE repository)
include_directories(domain PRIVATE model)