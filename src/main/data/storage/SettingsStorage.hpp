//
// Created by nbirdper on 06.02.2022.
//

#pragma once

#include "VirtualServer.hpp"
#include "SettingTypes.hpp"

class SettingsStorage {

private:
	MapHostVectorVirtualServers virtualServers;

public:
	const MapHostVectorVirtualServers& getVirtualServers() const;

	void addVirtualServerByHost(const std::string& host,
								VirtualServer* server);

	void setVirtualServers(MapHostVectorVirtualServers* virtualServers);

	void addLocation(const std::string& uri,
					 const VirtualServer* server);

	VirtualServer& findVirtualServer(const std::string& host,
									 const std::string& serverName) const;

//	Location* findLocationByUri(std::vector<Location*>* locations,
//								const std::string& uriRequest) const;
private:
	const std::vector<VirtualServer*>* getVirtualServersByHost(
										const std::string& host) const;

	VirtualServer&
	getVirtualServerByServerNameOrDefault(const std::vector<VirtualServer*>& servers,
										  const std::string& serverName) const;

//	size_t getLengthMatch(const std::string& uriLocation,
//						  const std::string& uriRequest) const;
};