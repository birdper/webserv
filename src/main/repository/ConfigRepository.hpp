//
// Created by nbirdper on 20.02.2022.
//

#pragma once

#include <iostream>
#include "usings.hpp"
#include "Config.hpp"

class ConfigRepository {

public:
    virtual VirtualServer& getServerConfig(const string& ip,
                                           const string& port,
                                           const string& serverName) const = 0;

    virtual Config* findLocationConfigByUri(const VirtualServer& virtualServer,
                                            const string& requestUri) const = 0;

    virtual std::vector<std::pair<string, int> > getHostsForBind() const = 0;
};


