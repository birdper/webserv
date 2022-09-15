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
                                                const string& requestUri) const {

    const std::vector<Location*>& locations = virtualServer.getLocations();

    Location* mostLengthMatch;
    Location* currentLocation;
    size_t maxLength = 0;
    for (int i = 0; i < locations.size(); ++i) {
        currentLocation = locations[i];
        std::string locationUri = currentLocation->getParameters()->uri;
//		TODO implement location search by uri
        size_t lengthMatch = getLengthMatch(locationUri, requestUri);
        if (lengthMatch > maxLength) {
            maxLength = lengthMatch;
            mostLengthMatch = currentLocation;
        }
    }
    if (maxLength)
        return new Config(mostLengthMatch->getParameters());
    return nullptr;
}

size_t ConfigRepositoryImpl::getLengthMatch(const std::string& locationUri,
                                            const std::string& requestUri) const {
    size_t length = 0;
    for (int i = 0; i < locationUri.length(); ++i) {
        if (locationUri[i] == requestUri[i])
            ++length;
    }
    return length;
}
