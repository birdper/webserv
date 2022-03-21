//
// Created by a19665881 on 21.03.2022.
//

#pragma once

#include <iostream>

#include <ctime>

#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/event.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <queue>
#include <unordered_map>

#define QUEUE_SIZE 1024
#define INVALID_SOCKET -1

class HttpServer {

private:
//	static const int QUEUE_SIZE = 1024;
//	static const int INVALID_SOCKET = -1;
	/** Server Socket */
	int listenPort;
	int listenSocketDescriptor; // Descriptor for the listening socket
	struct sockaddr_in serverAddr; // Structure for the server address
//	int dropUid; // setuid to this after bind()
//	int dropGid; // setgid to this after bind()

	// Kqueue
	struct timespec kqTimeout = {2, 0}; // Block for 2 seconds and 0ns at the most
	int kqfd;
	struct kevent events[QUEUE_SIZE]; // Events that have triggered a filter in the kqueue (max QUEUE_SIZE at a time)

	// Client map, maps Socket descriptor to Client object
	std::unordered_map<int, Client*> clients;

	// Resources / File System
	std::vector<ResourceHost*> hostList; // Contains all ResourceHosts
	std::unordered_map<std::string, ResourceHost*> vhosts; // Virtual hosts. Maps a host string to a ResourceHost to service the request

public:
	bool canRun;

	bool start();
	void process();
	void stop();

private:
	// Connection processing
	void updateEvent(int ident, short filter, u_short flags, u_int fflags, int data, void *udata);
	void acceptConnection();
	Client *getClient(int clfd);
	void disconnectClient(Client* client, bool mapErase = true);
	void readClient(Client* client, int data_len); // Client read event
	bool writeClient(Client* client, int avail_bytes); // Client write event
	ResourceHost* getResourceHostForRequest(HTTPRequest* req);

//	TODO move to RequestHandler
	// Request handling
	void handleRequest(Client* client, HTTPRequest* req);
	void handleGet(Client* client, HTTPRequest* req);
	void handleOptions(Client* client, HTTPRequest* req);
	void handleTrace(Client* client, HTTPRequest* req);

	// Response
	void sendStatusResponse(Client* client, int status, std::string msg = "");
	void sendResponse(Client* client, HTTPResponse* resp, bool disconnect);
};


