//
// Created by a19665881 on 21.03.2022.
//

#include "QueueServer.hpp"

bool QueueServer::start() {
	canRun = false;

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
	// Bind: Assign the address to the socket
	bind();

	// Optionally drop uid/gid if specified

	// Listen: Put the socket in a listening state, ready to accept connections
	// Accept a backlog of the OS Maximum connections in the queue

	// Setup kqueue
//	kqfd = kqueue();
//	if (kqfd == -1) {
//		std::cout << "Could not create the kernel event queue!" << std::endl;
//		return false;
//	}
//
//	// Have kqueue watch the listen socket
//	updateEvent(listenSocket, EVFILT_READ, EV_ADD, 0, 0, NULL);
//
//	canRun = true;
//	std::cout << "Server ready. Listening on port " << listenPort << "..." << std::endl;
//	return true;
}

