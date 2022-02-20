
set(CMAKE_CXX_STANDARD 98)

add_library(domain
        model/Config.hpp model/Config.cpp
        model/Parameters.hpp
        model/EnumMethod.hpp
        repository/SettingsRepository.hpp
        )

include_directories(domain PRIVATE repository)
include_directories(domain PRIVATE model)