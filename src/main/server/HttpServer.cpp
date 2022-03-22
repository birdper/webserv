//
// Created by a19665881 on 21.03.2022.
//

#include "HttpServer.hpp"

bool HttpServer::start() {
	canRun = false;

	std::vector<int> sockets;
	for (int i = 0; i < sockets.size(); ++i) {
		SocketKqueue& socketBuilder = *new SocketKqueue();
	}
	// Create a handle for the listening socket, TCP
	listenSocketDescriptor = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (listenSocketDescriptor == INVALID_SOCKET) {
		std::cout << "Could not create socket!" << std::endl;
		return false;
	}

	// Set socket as non blocking
	fcntl(listenSocketDescriptor, F_SETFL, O_NONBLOCK);

	// Populate the server address structure
	// modify to support multiple address families (bottom): http://eradman.com/posts/kqueue-tcp.html
	memset(&serverAddr, 0, sizeof(struct sockaddr_in)); // clear the struct

	serverAddr.sin_family = AF_INET; // Family: IP protocol
	serverAddr.sin_port = htons(listenPort); // Set the port (convert from host to netbyte order)
	serverAddr.sin_addr.s_addr = INADDR_ANY; // Let OS intelligently select the server's host address

	// Bind: Assign the address to the socket
	if (bind(listenSocketDescriptor, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) != 0) {
		std::cout << "Failed to bind to the address!" << std::endl;
		return false;
	}

	// Listen: Put the socket in a listening state, ready to accept connections
	// Accept a backlog of the OS Maximum connections in the queue
	if (listen(listenSocketDescriptor, SOMAXCONN) != 0) {
		std::cout << "Failed to put the socket in a listening state" << std::endl;
		return false;
	}

	// Setup kqueue
	kernelQueueFd = kqueue();
	if (kernelQueueFd == -1) {
		std::cout << "Could not create the kernel event queue!" << std::endl;
		return false;
	}

	// Have kqueue watch the listen socket
	updateEvent(listenSocketDescriptor, EVFILT_READ, EV_ADD, 0, 0, nullptr);

	canRun = true;
	std::cout << "Server ready. Listening on port " << listenPort << "..." << std::endl;
	return true;
}

/**
 * Update Event
 * Update the kqueue by creating the appropriate kevent structure
 * See kqueue documentation for parameter descriptions
 */
void HttpServer::updateEvent(int ident, short filter, u_short flags, u_int fflags, int data, void *udata) {
	struct kevent kev;
	EV_SET(&kev, ident, filter, flags, fflags, data, udata);
	kevent(kernelQueueFd, &kev, 1, nullptr, 0, nullptr);
}

/**
 * Server Process
 * Main server processing function that checks for any new connections or data to read on
 * the listening socket
 */
void HttpServer::process() {
	int nev = 0; // Number of changed events returned by kevent
	Client* client = nullptr;

	while (canRun) {
		// Get a list of changed socket descriptors with a read event triggered in events
		// Timeout set in the header
		nev = kevent(kernelQueueFd, nullptr, 0, events, QUEUE_SIZE, &kqTimeout);

		if (nev <= 0)
			continue;

		// Loop through only the sockets that have changed in the events array
		for (int i = 0; i < nev; i++) {
			if (events[i].ident == (unsigned int)listenSocketDescriptor) { // A client is waiting to connect
				acceptConnection();
			} else { // Client descriptor has triggered an event
				client = getClient(events[i].ident); // ident contains the clients socket descriptor
				if (client == nullptr) {
					std::cout << "Could not find client" << std::endl;
					// Remove socket events from kqueue
					updateEvent(events[i].ident, EVFILT_READ, EV_DELETE, 0, 0, nullptr);
					updateEvent(events[i].ident, EVFILT_WRITE, EV_DELETE, 0, 0, nullptr);

					// Close the socket descriptor
					close(events[i].ident);

					continue;
				}

				// Client wants to disconnect
				if (events[i].flags & EV_EOF) {
					disconnectClient(client, true);
					continue;
				}

				if (events[i].filter == EVFILT_READ) {
					//std::cout << "read filter " << events[i].data << " bytes available" << std::endl;
					// Read and process any pending data on the wire
					readClient(client, events[i].data); // data contains the number of bytes waiting to be read

					// Have kqueue disable tracking of READ events and enable tracking of WRITE events
					updateEvent(events[i].ident, EVFILT_READ, EV_DISABLE, 0, 0, nullptr);
					updateEvent(events[i].ident, EVFILT_WRITE, EV_ENABLE, 0, 0, nullptr);
				} else if (events[i].filter == EVFILT_WRITE) {
					//std::cout << "write filter with " << events[i].data << " bytes available" << std::endl;
					// Write any pending data to the client - writeClient returns true if there is additional data to send in the client queue
					if (!writeClient(client, events[i].data)) { // data contains number of bytes that can be written
						//std::cout << "switch back to read filter" << std::endl;
						// If theres nothing more to send, Have kqueue disable tracking of WRITE events and enable tracking of READ events
						updateEvent(events[i].ident, EVFILT_READ, EV_ENABLE, 0, 0, nullptr);
						updateEvent(events[i].ident, EVFILT_WRITE, EV_DISABLE, 0, 0, nullptr);
					}
				}
			}
		} // Event loop
	} // canRun
}

/**
 * Stop
 * Disconnect all clients and cleanup all server resources created in init()
 */
void HttpServer::stop() {
	canRun = false;

	if (listenSocketDescriptor != INVALID_SOCKET) {
		// Close all open connections and delete Client's from memory
		for (auto& x : clients)
			disconnectClient(x.second, false);

		// Clear the map
		clients.clear();

		// Remove listening socket from kqueue
		updateEvent(listenSocketDescriptor, EVFILT_READ, EV_DELETE, 0, 0, nullptr);

		// Shudown the listening socket and release it to the OS
		shutdown(listenSocketDescriptor, SHUT_RDWR);
		close(listenSocketDescriptor);
		listenSocketDescriptor = INVALID_SOCKET;
	}

	if (kernelQueueFd != -1) {
		close(kernelQueueFd);
		kernelQueueFd = -1;
	}

	std::cout << "Server shutdown!" << std::endl;
}

/**
 * Accept Connection
 * When a new connection is detected in runServer() this function is called. This attempts to accept the pending
 * connection, instance a Client object, and add to the client Map
 */
void HttpServer::acceptConnection() {
	// Setup new client with prelim address info
	sockaddr_in clientAddress;
	int clientAddrLen = sizeof(clientAddress);
	int clientFd = INVALID_SOCKET;

	// Accept the pending connection and retrive the client descriptor
	clientFd = accept(listenSocketDescriptor, (sockaddr*)&clientAddress, (socklen_t*)&clientAddrLen);
	if (clientFd == INVALID_SOCKET)
		return;

	// Set socket as non blocking
	fcntl(clientFd, F_SETFL, O_NONBLOCK);

	// Instance Client object
	Client *client = new Client(clientFd, clientAddress);

	// Add kqueue event to track the new client socket for READ and WRITE events
	updateEvent(clientFd, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, nullptr);
	updateEvent(clientFd, EVFILT_WRITE, EV_ADD | EV_DISABLE, 0, 0, nullptr); // Disabled initially

	// Add the client object to the client map
	clients.insert(std::pair<int, Client*>(clientFd, client));

	// Print the client's IP on connect
	std::cout << "[" << client->getClientIP() << "] connected" << std::endl;
}

/**
 * Get Client
 * Lookup client based on the socket descriptor number in the clientMap
 *
 * @param clfd Client socket descriptor
 * @return Pointer to Client object if found. nullptr otherwise
 */
Client* HttpServer::getClient(int clfd) {
	std::unordered_map<int, Client *>::iterator it = clients.find(clfd);

	if (it == clients.end())
		return nullptr;

	// Return a pointer to the client object
	return it->second;
}

/**
 * Disconnect Client
 * Close the client's socket descriptor and release it from the FD map, client map, and memory
 *
 * @param client Pointer to Client object
 * @param mapErase Wh
 *
 * en true, remove the client from the client map. Needed if operations on the
 * client map are being performed and we don't want to remove the map entry right away
 */
void HttpServer::disconnectClient(Client *client, bool mapErase) {
	if (client == nullptr)
		return;

	std::cout << "[" << client->getClientIP() << "] disconnected" << std::endl;

	// Remove socket events from kqueue
	updateEvent(client->getSocket(), EVFILT_READ, EV_DELETE, 0, 0, nullptr);
	updateEvent(client->getSocket(), EVFILT_WRITE, EV_DELETE, 0, 0, nullptr);

	// Close the socket descriptor
	close(client->getSocket());

	// Remove the client from the clientMap
	if (mapErase)
		clients.erase(client->getSocket());

	// Delete the client object from memory
	delete client;
}

/**
 * Read Client
 * Recieve data from a client that has indicated that it has data waiting. Pass recv'd data to handleRequest()
 * Also detect any errors in the state of the socket
 *
 * @param client Pointer to Client that sent the data
 * @param data_len Number of bytes waiting to be read
 */
void HttpServer::readClient(Client *client, int data_len) {
	if (client == nullptrptr)
		return;

	// If the read filter triggered with 0 bytes of data, client may want to disconnect
	// Set data_len to the Ethernet max MTU by default
	if (data_len <= 0)
		data_len = 1400;

	HTTPRequest* req;
	char* pData = new char[data_len];
	bzero(pData, data_len);

	// Receive data on the wire into pData
	int flags = 0;
	ssize_t lenRecv = recv(client->getSocket(), pData, data_len, flags);

	// Determine state of the client socket and act on it
	if (lenRecv == 0) {
		// Client closed the connection
		std::cout << "[" << client->getClientIP() << "] has opted to close the connection" << std::endl;
		disconnectClient(client, true);
	} else if (lenRecv < 0) {
		// Something went wrong with the connection
		// TODO: check perror() for the specific error message
		disconnectClient(client, true);
	} else {
		// Data received: Place the data in an HTTPRequest and pass it to handleRequest for processing
		req = new HTTPRequest((byte*)pData, lenRecv);
		handleRequest(client, req);
		delete req;
	}

	delete [] pData;
}

/**
 * Write Client
 * Client has indicated it is read for writing. Write avail_bytes number of bytes to the socket if the send queue has an item
 *
 * @param client Pointer to Client that sent the data
 * @param avail_bytes Number of bytes available for writing in the send buffer
 */
bool HttpServer::writeClient(Client* client, int avail_bytes) {
	if (client == nullptr)
		return false;

	int actual_sent = 0; // Actual number of bytes sent as returned by send()
	int attempt_sent = 0; // Bytes that we're attempting to send now
	int remaining = 0; // Size of data left to send for the item
	bool disconnect = false;
	byte* pData = nullptr;

	// The amount of available bytes to write, reported by the OS, cant really be trusted...
	if (avail_bytes > 1400) {
		// If the available amount of data is greater than the Ethernet MTU, cap it
		avail_bytes = 1400;
	} else if (avail_bytes == 0) {
		// Sometimes OS reports 0 when its possible to send data - attempt to trickle data
		// OS will eventually increase avail_bytes
		avail_bytes = 64;
	}

	SendQueueItem* item = client->nextInSendQueue();
	if (item == nullptr)
		return false;

	pData = item->getData();
	remaining = item->getSize() - item->getOffset();
	disconnect = item->getDisconnect();

	if (avail_bytes >= remaining) {
		// Send buffer is bigger than we need, rest of item can be sent
		attempt_sent = remaining;
	} else {
		// Send buffer is smaller than we need, send the amount thats available
		attempt_sent = avail_bytes;
	}

	// Send the data and increment the offset by the actual amount sent
	actual_sent = send(client->getSocket(), pData + (item->getOffset()), attempt_sent, 0);
	if (actual_sent >= 0)
		item->setOffset(item->getOffset() + actual_sent);
	else
		disconnect = true;

	//std::cout << "[" << client->getClientIP() << "] was sent " << actual_sent << " bytes " << std::endl;

	// SendQueueItem isnt needed anymore. Dequeue and delete
	if (item->getOffset() >= item->getSize())
		client->dequeueFromSendQueue();

	if (disconnect) {
		disconnectClient(client, true);
		return false;
	}

	return true;
}

/**
 * Handle Request
 * Process an incoming request from a Client. Send request off to appropriate handler function
 * that corresponds to an HTTP operation (GET, HEAD etc) :)
 *
 * @param client Client object where request originated from
 * @param req HTTPRequest object filled with raw packet data
 */
void HttpServer::handleRequest(Client *client, HTTPRequest* req) {
	// Parse the request
	// If there's an error, report it and send a server error in response
	if (!req->parse()) {
		std::cout << "[" << client->getClientIP() << "] There was an error processing the request of type: " << req->methodIntToStr(req->getMethod()) << std::endl;
		// std::cout << req->getParseError().c_str() << std::endl;
		sendStatusResponse(client, Status(BAD_REQUEST), req->getParseError());
		return;
	}

	std::cout << "[" << client->getClientIP() << "] " << req->methodIntToStr(req->getMethod()) << " " << req->getRequestUri() << std::endl;
	/*std::cout << "Headers:" << std::endl;
	for(int i = 0; i < req->getNumHeaders(); i++) {
		std::cout << "\t" << req->getHeaderStr(i) << std::endl;
	}
	std::cout << std::endl;*/

	// Send the request to the correct handler function
	switch (req->getMethod()) {
		case Method(HEAD):
		case Method(GET):
			handleGet(client, req);
			break;
		case Method(OPTIONS):
			handleOptions(client, req);
			break;
		case Method(TRACE):
			handleTrace(client, req);
			break;
		default:
			std::cout << "[" << client->getClientIP() << "] Could not handle or determine request of type " << req->methodIntToStr(req->getMethod()) << std::endl;
			sendStatusResponse(client, Status(NOT_IMPLEMENTED));
			break;
	}
}

/**
 * Handle Get or Head
 * Process a GET or HEAD request to provide the client with an appropriate response
 *
 * @param client Client requesting the resource
 * @param req State of the request
 */
void HttpServer::handleGet(Client* client, HTTPRequest* req) {
	std::string uri;
	Resource* r = nullptr;
	ResourceHost* resHost = this->getResourceHostForRequest(req);

	// ResourceHost couldnt be determined or the Host specified by the client was invalid
	if (resHost == nullptr) {
		sendStatusResponse(client, Status(BAD_REQUEST), "Invalid/No Host specified");
		return;
	}

	// Check if the requested resource exists
	uri = req->getRequestUri();
	r = resHost->getResource(uri);

	if (r != nullptr) { // Exists
		std::cout << "[" << client->getClientIP() << "] " << "Sending file: " << uri << std::endl;

		HTTPResponse* resp = new HTTPResponse();
		resp->setStatus(Status(OK));
		resp->addHeader("Content-Type", r->getMimeType());
		resp->addHeader("Content-Length", r->getSize());

		// Only send a message body if it's a GET request. Never send a body for HEAD
		if (req->getMethod() == Method(GET))
			resp->setData(r->getData(), r->getSize());

		bool dc = false;

		// HTTP/1.0 should close the connection by default
		if (req->getVersion().compare(HTTP_VERSION_10) == 0)
			dc = true;

		// If Connection: close is specified, the connection should be terminated after the request is serviced
		std::string connection_val = req->getHeaderValue("Connection");
		if (connection_val.compare("close") == 0)
			dc = true;

		sendResponse(client, resp, dc);
		delete resp;
		delete r;
	} else { // Not found
		std::cout << "[" << client->getClientIP() << "] " << "File not found: " << uri << std::endl;
		sendStatusResponse(client, Status(NOT_FOUND));
	}
}

/**
 * Handle Options
 * Process a OPTIONS request
 * OPTIONS: Return allowed capabilties for the server (*) or a particular resource
 *
 * @param client Client requesting the resource
 * @param req State of the request
 */
void HttpServer::handleOptions(Client* client, HTTPRequest* req) {
	// For now, we'll always return the capabilities of the server instead of figuring it out for each resource
	std::string allow = "HEAD, GET, OPTIONS, TRACE";

	HTTPResponse* resp = new HTTPResponse();
	resp->setStatus(Status(OK));
	resp->addHeader("Allow", allow.c_str());
	resp->addHeader("Content-Length", "0"); // Required

	sendResponse(client, resp, true);
	delete resp;
}

/**
 * Handle Trace
 * Process a TRACE request
 * TRACE: send back the request as received by the server verbatim
 *
 * @param client Client requesting the resource
 * @param req State of the request
 */
void HttpServer::handleTrace(Client* client, HTTPRequest *req) {
	// Get a byte array representation of the request
	unsigned int len = req->size();
	byte* buf = new byte[len];
	bzero(buf, len);
	req->setReadPos(0); // Set the read position at the beginning since the request has already been read to the end
	req->getBytes(buf, len);

	// Send a response with the entire request as the body
	HTTPResponse* resp = new HTTPResponse();
	resp->setStatus(Status(OK));
	resp->addHeader("Content-Type", "message/http");
	resp->addHeader("Content-Length", len);
	resp->setData(buf, len);
	sendResponse(client, resp, true);

	delete resp;
	delete[] buf;
}

/**
 * Send Status Response
 * Send a predefined HTTP status code response to the client consisting of
 * only the status code and required headers, then disconnect the client
 *
 * @param client Client to send the status code to
 * @param status Status code corresponding to the enum in HTTPMessage.h
 * @param msg An additional message to append to the body text
 */
void HttpServer::sendStatusResponse(Client* client, int status, std::string msg) {
	HTTPResponse* resp = new HTTPResponse();
	resp->setStatus(Status(status));

	// Body message: Reason string + additional msg
	std::string body = resp->getReason();
	if (msg.length() > 0)
		body +=  ": " + msg;

	unsigned int slen = body.length();
	char* sdata = new char[slen];
	bzero(sdata, slen);
	strncpy(sdata, body.c_str(), slen);

	resp->addHeader("Content-Type", "text/plain");
	resp->addHeader("Content-Length", slen);
	resp->setData((byte*)sdata, slen);

	sendResponse(client, resp, true);

	delete resp;
}

/**
 * Send Response
 * Send a generic HTTPResponse packet data to a particular Client
 *
 * @param client Client to send data to
 * @param buf ByteBuffer containing data to be sent
 * @param disconnect Should the server disconnect the client after sending (Optional, default = false)
 */
void HttpServer::sendResponse(Client* client, HTTPResponse* resp, bool disconnect) {
	// Server Header
	resp->addHeader("Server", "httpserver/1.0");

	// Time stamp the response with the Date header
	std::string tstr;
	char tbuf[36] = {0};
	time_t rawtime;
	struct tm* ptm;
	time(&rawtime);
	ptm = gmtime(&rawtime);
	// Ex: Fri, 31 Dec 1999 23:59:59 GMT
	strftime(tbuf, 36, "%a, %d %b %Y %H:%M:%S GMT", ptm);
	tstr = tbuf;
	resp->addHeader("Date", tstr);

	// Include a Connection: close header if this is the final response sent by the server
	if (disconnect)
		resp->addHeader("Connection", "close");

	// Get raw data by creating the response (we are responsible for cleaning it up in process())
	byte* pData = resp->create();

	// Add data to the Client's send queue
	client->addToSendQueue(new SendQueueItem(pData, resp->size(), disconnect));
}

/**
 * Get Resource Host
 * Retrieve the appropriate ResourceHost instance based on the requested path
 *
 * @param req State of the request
 */
ResourceHost* HttpServer::getResourceHostForRequest(HTTPRequest* req) {
	// Determine the appropriate vhost
	ResourceHost* resHost = nullptr;
	std::string host = "";

	// Retrieve the host specified in the request (Required for HTTP/1.1 compliance)
	if (req->getVersion().compare(HTTP_VERSION_11) == 0) {
		host = req->getHeaderValue("Host");

		// All vhosts have the port appended, so need to append it to the host if it doesnt exist
		if (host.find(":") == std::string::npos) {
			host.append(":" + std::to_string(listenPort));
		}

		std::unordered_map<std::string, ResourceHost*>::const_iterator it = vhosts.find(host);

		if (it != vhosts.end())
			resHost = it->second;
	} else {
		// Temporary: HTTP/1.0 are given the first ResouceHost in the hostList
		// TODO: Allow admin to specify a 'default resource host'
		if (hostList.size() > 0)
			resHost = hostList[0];
	}

	return resHost;
}

