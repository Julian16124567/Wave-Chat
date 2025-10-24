#ifndef SERVER_HPP
#define SERVER_HPP

#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
using std::string;

struct Server {
    int sock = -1;
    int clientSock = -1;
    sockaddr_in address{};
    unsigned short port = 9090;
    bool running = false;

    bool buildSocket();
    bool acceptClient();
    void handleClient(int clientFd);
};

#endif