#include <iostream>
#include "socket.h"
using namespace std;

stdsock::ConnectionPoint::ConnectionPoint(int port, int backlog) : Socket(port), backlog(backlog) {
}

stdsock::StreamSocket *stdsock::ConnectionPoint::accept() {
    struct sockaddr_in from;
    socklen_t l = sizeof(from);
    int clientfd = ::accept(sockfd, (struct sockaddr*)&from, &l);

    return new stdsock::StreamSocket(clientfd, from);
}

int stdsock::ConnectionPoint::bind() {
    int optval = 1;
#ifdef _WIN32
    if (::setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (char*) &optval, sizeof(int)) != 0) {
#else
    if (::setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int)) != 0) {
#endif
        perror("[-] Error in setting options: ");
        close();
        exit(errno);
    }

    if (::bind(sockfd, (struct sockaddr*)&address, sizeof(struct sockaddr)) != 0) {
        close();
        return errno;
    }

    return 0;
}

int stdsock::ConnectionPoint::listen() {
    if (::listen(sockfd, backlog) != 0) {
        close();
        return errno;
    }

    return 0;
}

int stdsock::ConnectionPoint::init() {
    //sockfd = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (sockfd == -1) {
        return errno;
    }

    int err;
    if ( (err=bind()) != 0)
        return err;

    if ( (err=listen()) != 0)
        return err;

    return 0;
}

stdsock::Socket::Socket(const std::string& IP, int port)
{
    int optval = 1;

#ifdef _WIN32
    WSADATA wsa_data;
    WSAStartup(MAKEWORD(2, 2), &wsa_data);
    sockfd = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
#else
    sockfd = ::socket(AF_INET, SOCK_STREAM, 0);
#endif

    if (sockfd == -1) {
        perror("[-] Error in Connexion: ");
        exit(errno);
    }
#ifdef _WIN32
    if (::setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (char*)&optval, sizeof(int)) != 0) {
#else
    if (::setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int)) != 0) {
#endif
        perror("[-] Error in setting options: ");
        close();
        exit(errno);
    }

    // Configuration de l'adresse de transport
    address.sin_family = AF_INET;           // socket type
    address.sin_port = htons(port);         // port, converted in net style
    //    address.sin_addr.s_addr = INADDR_ANY; // any valid address in the domain
    //bzero(&(address.sin_zero), 8);          // zeroing the remaining of the structure
    memset(&(address.sin_zero), 0, 8);          // zeroing the remaining of the structure

    // 127.0.0.1 Convert IPv4 and IPv6 addresses from text to binary form
    //    if(inet_aton(IP.c_str(), &address.sin_addr)<=0)
    if (inet_pton(address.sin_family,IP.c_str(), &address.sin_addr) <= 0)
    {
        perror("\nInvalid address/ Address not supported \n");
        exit(errno);
    }

}

stdsock::Socket::Socket(int port)
{
    int optval = 1;

#ifdef _WIN32
    WSADATA wsa_data;
    WSAStartup(MAKEWORD(2, 2), &wsa_data);
    sockfd = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
#else
    sockfd = ::socket(AF_INET, SOCK_STREAM, 0);
#endif

    if (sockfd == -1) {
        perror("[-] Error in Connexion: ");
        exit(errno);
    }
#ifdef _WIN32
    if (::setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (char*)&optval, sizeof(int)) != 0) {
#else
    if (::setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int)) != 0) {
#endif
        perror("[-] Error in setting options: ");
        close();
        exit(errno);
    }

    // Transport address Configuration
    address.sin_family = AF_INET;       // socket type
    address.sin_port = htons(port);     // port, converted in net style
    address.sin_addr.s_addr = INADDR_ANY; // any valid address in the domain
    //    bzero(&(address.sin_zero), 8);        // zeroing the remaining of the structure
    memset(&(address.sin_zero),0, 8);        // zeroing the remaining of the structure

}

stdsock::Socket::Socket(int sockfd, const struct sockaddr_in& addr)
    : sockfd(sockfd), address(addr)
{
#ifdef _WIN32
    WSADATA wsa_data;
    WSAStartup(MAKEWORD(2,2), &wsa_data);
#endif
}

stdsock::StreamSocket::StreamSocket(int port)
    : stdsock::Socket(port)
{}

stdsock::StreamSocket::StreamSocket(const std::string &IP, int port)
    : stdsock::Socket(IP,port)
{}

stdsock::StreamSocket::StreamSocket(int sockfd, const struct sockaddr_in& addr)
    : stdsock::Socket(sockfd,addr)
{}

stdsock::Socket::~Socket()
{
    close();

#ifdef _WIN32
    WSACleanup();
#endif

}

int stdsock::StreamSocket::connect() {
    ::connect(sockfd, (struct sockaddr*)&address, sizeof(address));
    return errno;
}

void stdsock::Socket::close() {
#ifdef _WIN32
    ::closesocket(sockfd);
#else
    if (sockfd == -1) {
        return;
    }
    // call std::basic_fstream<CharT,Traits>::close
    ::close(sockfd);
#endif
}

#ifdef _WIN32
void stdsock::StreamSocket::setBlocking() {
    unsigned long opts = 0;
    ioctlsocket(sockfd, FIONBIO, &opts);
}

void stdsock::StreamSocket::setNonBlocking() {
    unsigned long opts = 1;
    ioctlsocket(sockfd, FIONBIO, &opts);
}
#else
void stdsock::StreamSocket::setBlocking() {
    int opts = fcntl(sockfd, F_GETFL);
    opts = opts & (~O_NONBLOCK);
    fcntl(sockfd, F_SETFL, opts);
}

void stdsock::StreamSocket::setNonBlocking() {
    fcntl(sockfd, F_SETFL, O_NONBLOCK);
}
#endif

int stdsock::StreamSocket::read(std::string &msg) {

    int bytes_total = 0;
    char buffer[DEFAULT_SOCKET_BUFFER];

    int bytes_read = recv(sockfd, buffer, DEFAULT_SOCKET_BUFFER, 0);

    if (bytes_read <= 0) {
        return bytes_read;
    }

    msg.append(std::string(buffer, 0, bytes_read));
    bytes_total += bytes_read;

    // set non-blocking.
    setNonBlocking();

    while (bytes_read > 0) {
        memset(buffer, 0, DEFAULT_SOCKET_BUFFER);
        bytes_read = recv(sockfd, buffer, DEFAULT_SOCKET_BUFFER, 0);

        if (bytes_read < 0) {
            break;
        }

        msg.append(std::string(buffer, 0, bytes_read));
        bytes_total += bytes_read;
    }

    // set back to blocking
    setBlocking();

    return bytes_total;
}

int stdsock::StreamSocket::read(char* buf, int len) {
    return ::recv(sockfd, buf, len, 0);
}

int stdsock::StreamSocket::send(const std::string &msg) {
    return send(msg.c_str(), msg.length(), 0);
}

int stdsock::StreamSocket::send(const char* buf, int len, int flags) {
    return ::send(sockfd, buf, len, flags);
}
