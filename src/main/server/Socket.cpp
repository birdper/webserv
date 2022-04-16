

#include "Socket.hpp"

int Socket::initSocket(int listenPort, in_addr_t ip) {

	int ret;

	int socketDescriptor = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (socketDescriptor == InvalidSocket) {
		std::cout << "Could not create socket!" << std::endl;
		return InvalidSocket;
	}

	fcntl(socketDescriptor, F_SETFL, O_NONBLOCK);
	struct sockaddr_in socketAddress = initAddress(listenPort, ip);

	ret = bind(socketDescriptor, (struct sockaddr*)&socketAddress, sizeof(socketAddress));
	if (ret != 0) {
		std::cout << "Failed to bind to the address!" << std::endl;
		return InvalidSocket;
	}

	// Listen: Put the socket in a listening state, ready to accept connections
	// Accept a backlog of the OS Maximum connections in the queue
	ret = listen(socketDescriptor, SOMAXCONN);
	if (ret != 0) {
		std::cout << "Failed to put the socket in a listening state" << std::endl;
		return InvalidSocket;
	}
	return socketDescriptor;
}

struct sockaddr_in Socket::initAddress(int listenPort, in_addr_t ip) {
	struct sockaddr_in socketAddress;

	// modify to support multiple address families (bottom): http://eradman.com/posts/kqueue-tcp.html
//	memset(&socketAddress, 0, sizeof(struct sockaddr_in));

	socketAddress.sin_family = AF_INET;
	socketAddress.sin_port = htons(listenPort);
	socketAddress.sin_addr.s_addr = ip; // Let OS intelligently select the server's host address
	return socketAddress;
}
