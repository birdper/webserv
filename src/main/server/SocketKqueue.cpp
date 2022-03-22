

#include "SocketKqueue.hpp"

int SocketKqueue::init(int listenPort, in_addr_t ip = INADDR_ANY) {

	int ret = 0;
	int listenSocketDescriptor = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (listenSocketDescriptor == INVALID_SOCKET) {
		std::cout << "Could not create socket!" << std::endl;
		return INVALID_SOCKET;
	}

	fcntl(listenSocketDescriptor, F_SETFL, O_NONBLOCK);
	initAddress(listenPort, ip);

	ret = bind(listenSocketDescriptor, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
	if (ret != 0) {
		std::cout << "Failed to bind to the address!" << std::endl;
		return INVALID_SOCKET;
	}

	// Listen: Put the socket in a listening state, ready to accept connections
	// Accept a backlog of the OS Maximum connections in the queue
	ret = listen(listenSocketDescriptor, SOMAXCONN);
	if (ret != 0) {
		std::cout << "Failed to put the socket in a listening state" << std::endl;
		return INVALID_SOCKET;
	}
	return listenSocketDescriptor;
}

void SocketKqueue::initAddress(int listenPort, in_addr_t ip) {
	// modify to support multiple address families (bottom): http://eradman.com/posts/kqueue-tcp.html
	memset(&serverAddr, 0, sizeof(struct sockaddr_in));

	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(listenPort);
	serverAddr.sin_addr.s_addr = ip; // Let OS intelligently select the server's host address
}