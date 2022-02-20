//
// Created by nbirdper on 20.02.2022.
//

#pragma once

#include <iostream>
#include "Config.hpp"

class SettingsRepository {

public:
	virtual Config* getConfig(const std::string& uriRequest,
					  const std::string& host,
					  const std::string& serverName) const = 0;
};


