//
// Created by nbirdper on 20.02.2022.
//

#include "ConfigRepositoryImpl.hpp"

ConfigRepositoryImpl::ConfigRepositoryImpl(ConfigStorage* storage) :
        storage(storage) {
}

std::vector<std::pair<std::string, int> > ConfigRepositoryImpl::getHostsForBind() const {
    std::vector<std::pair<std::string, int> > hosts;
    MapHostVectorVirtualServers allServers = storage->getVirtualServers();
    for (MapHostVectorVirtualServers::iterator it = allServers.begin(); it != allServers.end(); ++it) {
        VirtualServer* virtualServer = it->second[0];
        hosts.push_back(std::make_pair(virtualServer->getIp(), virtualServer->getPort()));
    }
    return hosts;
}

std::vector<VirtualServer*> ConfigRepositoryImpl::getVirtualServersForBind() const {
    std::vector<VirtualServer*> hosts;
    MapHostVectorVirtualServers allServers = storage->getVirtualServers();
    for (MapHostVectorVirtualServers::iterator it = allServers.begin(); it != allServers.end(); ++it) {
        hosts.push_back(it->second[0]);
    }
    return hosts;
}

VirtualServer&
ConfigRepositoryImpl::getServerConfig(const std::string& ip,
                                      const std::string& port,
                                      const std::string& serverName) const {

    std::vector<VirtualServer*> serversByPort = storage->getVirtualServersByPort(port);

    std::vector<VirtualServer*> serversSameIp;
    for (int i = 0; i < serversByPort.size(); ++i) {
        if (serversByPort[i]->getIp() == ip) {
            serversSameIp.push_back(serversByPort[i]);
        }
    }
    if (!serversSameIp.empty() && !serverName.empty()) {
        for (int i = 0; i < serversSameIp.size(); ++i) {
            if (serversSameIp[i]->isContainServerName(serverName))
                return *serversSameIp[i];
        }
    }
    return *serversSameIp[0];
}

Config* ConfigRepositoryImpl::findLocationConfigByUri(const VirtualServer& virtualServer,
                                                      string requestUri) const {

    const std::vector<Location*>& locations = virtualServer.getLocations();

    Location* rootLocation;
    Location* mostLengthMatch;
    size_t maxLength = 0;
    std::vector<string> reqs = Utils::split(requestUri, "/");

    for (int i = 0; i < locations.size(); ++i) {
        Location* currentLocation = locations[i];
        std::vector<string> locs = Utils::split(currentLocation->getUri(), "/");

        if (locs.size() > reqs.size()) {
            break;
        }
        if (isAnyRequest(locs)) {
            if (rootLocation == nullptr) {
                rootLocation = currentLocation;
            }
        } else {
            if (findMatchUri(reqs, locs)) {
                if (locs.size() > maxLength) {
                    maxLength = locs.size();
                    mostLengthMatch = currentLocation;
                }
            }
        }
    }

    if (mostLengthMatch != nullptr) {
        return new Config(mostLengthMatch->getParameters(), true);
    } else if (rootLocation != nullptr) {
        return new Config(rootLocation->getParameters(), true);
    }
    return nullptr;
}

bool ConfigRepositoryImpl::findMatchUri(const std::vector<string>& reqs, const std::vector<string>& locs) const {
    for (int i = 0; i < locs.size(); ++i) {
        if (locs[i] != reqs[i]) {
            return false;
        }
    }
    return true;
}

bool ConfigRepositoryImpl::isAnyRequest(const std::vector<string>& locs) const {
    return locs.empty();
}
