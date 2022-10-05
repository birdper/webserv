//
// Created by nbirdper on 20.02.2022.
//

#pragma once

#include "usings.hpp"
#include "Utils.hpp"
#include "SettingTypes.hpp"
#include "ConfigStorage.hpp"
#include "ConfigRepository.hpp"
#include "MimeTypesRepo.hpp"
#include "DefaultStatusCodeRepo.hpp"

class ConfigRepositoryImpl : public ConfigRepository {
private:
    ConfigStorage* storage;
	DefaultStatusCodeRepo statusCodeRepo;
	MimeTypesRepo mimeTypesRepo;


public:
    explicit ConfigRepositoryImpl(ConfigStorage* storage);

    std::vector<std::pair<string, int> > getHostsForBind() const;

    VirtualServer& getServerConfig(const string& ip,
                                   const string& port,
                                   const string& serverName) const;

    Config* findLocationConfigByUri(const VirtualServer& virtualServer,
                                    const string& requestUri);

	Config* getLocationConfig(Parameters& parameters);
	Config* getServerConfig(Parameters& parameters);
private:
    std::vector<VirtualServer*> getVirtualServersForBind() const;
};


