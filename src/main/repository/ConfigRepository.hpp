//
// Created by nbirdper on 20.02.2022.
//

#pragma once

#include <iostream>
#include "Config.hpp"

class ConfigRepository {

public:
	virtual Config* getConfig(const std::string& uriRequest,
					  const std::string& host,
					  const std::string& serverName) const = 0;

    virtual std::vector<std::pair<std::string, int> > getHostsForBind() const = 0;
};


