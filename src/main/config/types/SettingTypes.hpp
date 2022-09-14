//
// Created by nbirdper on 06.02.2022.
//

#pragma once

#include <map>
#include <vector>
#include "VirtualServer.hpp"

/**
 * @key: port
 * @value: vector of VirtualServer*
 */
typedef std::map<std::string, std::vector<VirtualServer*> > MapHostVectorVirtualServers;