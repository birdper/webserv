

#include "Socket.hpp"

Socket::Socket(const string& ip, int port) :
        socketDescriptor(socketDescriptor),
        ip(ip),
        port(port) {
    init();
}

void Socket::init() {
    openSocket();
    setNonblockMode();

    struct sockaddr_in address = initAddress();
    bindToAddress(address);

    setAddressReuseMode();

    startListening();
}

void Socket::openSocket() {
    socketDescriptor = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    checkError(socketDescriptor, "openSocket()");
}

void Socket::checkError(int result, const std::string& nameFunFromError) {
    if (result < 0) {
        perror(nameFunFromError.c_str());
        exit(EXIT_FAILURE);
    }
}

void Socket::setAddressReuseMode() {
    int onActiveFlag = 1;
    int result = setsockopt(socketDescriptor,
                            SOL_SOCKET,
                            SO_REUSEADDR,
                            &onActiveFlag,
                            sizeof(onActiveFlag));
    checkError(result, "setAddressReuseMode()");
}

void Socket::setNonblockMode() {
    int result = fcntl(socketDescriptor, F_SETFL, O_NONBLOCK);
    checkError(result, "setNonblockMode()");
}

struct sockaddr_in Socket::initAddress() {
    struct sockaddr_in socketAddress;

//	TODO delete comment if unnecessary
    // modify to support multiple address families (bottom): http://eradman.com/posts/kqueue-tcp.html
//	memset(&socketAddress, 0, sizeof(struct sockaddr_in));

    socketAddress.sin_family = AF_INET;
    socketAddress.sin_port = htons(port);
    socketAddress.sin_addr.s_addr = inet_addr(ip.c_str()); // Let OS intelligently select the server's host address
    return socketAddress;
}

void Socket::bindToAddress(struct sockaddr_in address) {
    int result = bind(socketDescriptor, (struct sockaddr*) &address, sizeof(sockaddr));

    checkError(result, "bindToAddress() " +
                       std::string(inet_ntoa(address.sin_addr)) + ":" +
                       std::to_string(ntohs(address.sin_port)));
}

void Socket::startListening() {
    int result = listen(socketDescriptor, SOMAXCONN);
    checkError(result, "startListening()");
}

int Socket::getSocketDescriptor() const {
    return socketDescriptor;
}

const std::string& Socket::getIp() const {
    return ip;
}

string Socket::getPort() const {
    return std::to_string(port);
}
