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

	Config* getConfig(const std::string& requestUri,
					  const std::string& host,
					  const std::string& serverName) const;
	std::vector<std::pair<std::string, int> > getHostsForBind() const;

private:

	Location* findLocationByUri(std::vector<Location*>& locations,
								const std::string& requestUri) const;
	size_t getLengthMatch(const std::string& locationUri,
						  const std::string& requestUri) const;
	std::vector<VirtualServer*> getVirtualServersForBind() const;
};


