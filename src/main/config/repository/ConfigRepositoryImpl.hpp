//
// Created by nbirdper on 20.02.2022.
//

#pragma once

#include "SettingTypes.hpp"
#include "ConfigStorage.hpp"
#include "ConfigRepository.hpp"

class ConfigRepositoryImpl : public ConfigRepository {
private:
	ConfigStorage* storage;

public:
	explicit ConfigRepositoryImpl(ConfigStorage* storage);

    std::vector<std::pair<std::string, int> > getHosts() const;

    std::vector<std::string> getHostsVirtualServers() const;

	Config* getConfig(const std::string& requestUri,
					  const std::string& host,
					  const std::string& serverName) const;

private:
	Location* findLocationByUri(std::vector<Location*>& locations,
								const std::string& requestUri) const;

	size_t getLengthMatch(const std::string& locationUri,
						  const std::string& requestUri) const;
};


