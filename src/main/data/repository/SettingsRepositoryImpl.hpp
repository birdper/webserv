//
// Created by nbirdper on 20.02.2022.
//

#pragma once

#include "SettingTypes.hpp"
#include "SettingsStorage.hpp"
#include "SettingsRepository.hpp"

class SettingsRepositoryImpl : public SettingsRepository {

private:
	SettingsStorage* storage;

public:
	explicit SettingsRepositoryImpl(SettingsStorage* storage);

	std::vector<std::string> getHostsVirtualServers() const;

	Config* getConfig(const std::string& uriRequest,
					  const std::string& host,
					  const std::string& serverName) const;

private:
	Location* findLocationByUri(std::vector<Location*>* locations,
								const std::string& uriRequest) const;

	size_t getLengthMatch(const std::string& uriLocation,
						  const std::string& uriRequest) const;
};


