//
// Created by a19665881 on 20.02.2022.
//

#include "SettingsRepositoryImpl.hpp"

SettingsRepositoryImpl::SettingsRepositoryImpl(SettingsStorage* storage)
		: storage(storage) {}

Config* SettingsRepositoryImpl::getConfig(const std::string& uriRequest,
										  const std::string& host,
										  const std::string& serverName) const {
	VirtualServer* virtualServer = storage->findVirtualServerByHost(host,
																	serverName);
	std::vector<Location*> locations = virtualServer->getLocations();

//	TODO если нет подходящего локейшена то что?
	Location* location = storage->findLocationByUri(&locations, uriRequest);
	return location == nullptr ? nullptr
							   : new Config(location->getParameters());
}

std::vector<VirtualServer*> SettingsRepositoryImpl::getVirtualServers() const {
//	TODO yet not implement
	return std::vector<VirtualServer*>();
}
