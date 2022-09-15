
#include "Socket.hpp"

Socket::Socket(const string& ip, int port) :
        _ip(ip),
        _port(port) {
    init();
}

Socket& Socket::operator=(Socket other) {
    if (this != &other) {
        _socketDescriptor = other._socketDescriptor;
        _ip = other._ip;
        _port = other._port;
    }
    return *this;
}

Socket::Socket(const Socket& other) :
        _socketDescriptor(other._socketDescriptor),
        _ip(other._ip),
        _port(other._port) {
}

Socket* Socket::getInstance(const string& ip, int port) {
    return new Socket(ip, port);
}


void Socket::init() {
    openSocket();

    setNonblockMode(_socketDescriptor);
    setAddressReuseMode();

    struct sockaddr_in address = initAddress();
    bindToAddress(address);

    startListening();
}

void Socket::openSocket() {
    _socketDescriptor = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    checkError(_socketDescriptor, "openSocket()");
}

void Socket::checkError(int result, const std::string& nameFunFromError) {
    if (result < 0) {
        perror(nameFunFromError.c_str());
        exit(EXIT_FAILURE);
    }
}

void Socket::setAddressReuseMode() {
    int onActiveFlag = 1;
    int result = setsockopt(_socketDescriptor,
                            SOL_SOCKET,
                            SO_REUSEADDR,
                            &onActiveFlag,
                            sizeof(onActiveFlag));
    checkError(result, "setAddressReuseMode()");
}

void Socket::setNonblockMode(int socketDescriptor) {
    int result = fcntl(socketDescriptor, F_SETFL, O_NONBLOCK);
    checkError(result, "setNonblockMode()");
}

struct sockaddr_in Socket::initAddress() {
    struct sockaddr_in socketAddress;

    socketAddress.sin_family = AF_INET;
    socketAddress.sin_port = htons(_port);
    socketAddress.sin_addr.s_addr = inet_addr(_ip.c_str());
    return socketAddress;
}

void Socket::bindToAddress(struct sockaddr_in address) {
    int result = bind(_socketDescriptor, (struct sockaddr*) &address, sizeof(sockaddr));

    checkError(result, "bindToAddress() " +
                       std::string(inet_ntoa(address.sin_addr)) + ":" +
                       std::to_string(ntohs(address.sin_port)));
}



void Socket::startListening() {
    int result = listen(_socketDescriptor, SOMAXCONN);
    checkError(result, "startListening()");
}

int Socket::getSocketDescriptor() const {
    return _socketDescriptor;
}

const std::string& Socket::getIp() const {
    return _ip;
}



string Socket::getPortString() const {
    return std::to_string(_port);
}
