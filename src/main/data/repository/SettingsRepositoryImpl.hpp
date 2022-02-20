//
// Created by a19665881 on 20.02.2022.
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

	Config* getConfig(const std::string& uriRequest,
					  const std::string& host,
					  const std::string& serverName) const;

	std::vector<VirtualServer*> getVirtualServers() const;
};


