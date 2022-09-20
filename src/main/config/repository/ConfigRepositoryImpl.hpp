//
// Created by nbirdper on 20.02.2022.
//

#pragma once

#include "usings.hpp"
#include "Utils.hpp"
#include "SettingTypes.hpp"
#include "ConfigStorage.hpp"
#include "ConfigRepository.hpp"

class ConfigRepositoryImpl : public ConfigRepository {
private:
	ConfigStorage* storage;

public:
	explicit ConfigRepositoryImpl(ConfigStorage* storage);

	std::vector<std::pair<string, int> > getHostsForBind() const;

    VirtualServer& getServerConfig(const string& ip,
                                   const string& port,
                                   const string& serverName) const;

	Config* findLocationConfigByUri(const VirtualServer& virtualServer,
                                    string requestUri) const;

private:
	std::vector<VirtualServer*> getVirtualServersForBind() const;
    bool isAnyRequest(const std::vector<string>& locs) const;
    bool findMatchUri(const std::vector<string>& reqs, const std::vector<string>& locs) const;
};


