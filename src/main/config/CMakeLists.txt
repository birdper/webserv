

set(CMAKE_CXX_STANDARD 98)

set(MODULE_NAME config)

add_library(${MODULE_NAME}
        model/DefaultParameters.cpp model/DefaultParameters.hpp
        model/Location.cpp model/Location.hpp
        model/VirtualServer.cpp model/VirtualServer.hpp
        storage/ConfigStorage.cpp storage/ConfigStorage.hpp
        repository/DefaultStatusCodeRepo.cpp repository/DefaultStatusCodeRepo.hpp
        repository/ConfigRepositoryImpl.cpp repository/ConfigRepositoryImpl.hpp
        )

target_include_directories(${MODULE_NAME} PUBLIC
        ../util
        model
        types
        storage
        repository
        ../model
        ../repository
        )