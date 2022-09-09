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

	std::vector<std::string> getHostsVirtualServers() const;

	Config* getConfig(const std::string& uriRequest,
					  const std::string& host,
					  const std::string& serverName) const;

private:
	Location* findLocationByUri(std::vector<Location*>& locations,
								const std::string& uriRequest) const;

	size_t getLengthMatch(const std::string& uriLocation,
						  const std::string& uriRequest) const;
};


