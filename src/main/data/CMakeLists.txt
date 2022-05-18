

set(CMAKE_CXX_STANDARD 98)


add_library(data
        model/DefaultParameters.cpp model/DefaultParameters.hpp
        model/Location.cpp model/Location.hpp
        model/VirtualServer.cpp model/VirtualServer.hpp
        storage/SettingsStorage.cpp storage/SettingsStorage.hpp
        repository/RepositoryErrorPages.cpp repository/RepositoryErrorPages.hpp
        repository/SettingsRepositoryImpl.cpp repository/SettingsRepositoryImpl.hpp
        repository/ClientRepository.cpp repository/ClientRepository.hpp
        )

target_include_directories(data PUBLIC model types storage repository ../domain/model ../domain/repository)
#target_include_directories(data PUBLIC types)
#target_include_directories(data PUBLIC storage)
#target_include_directories(data PUBLIC repository)
#target_include_directories(data PUBLIC ../domain/model)
#target_include_directories(data PUBLIC ../domain/repository)