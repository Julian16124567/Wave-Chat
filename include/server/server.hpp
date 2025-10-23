#ifndef SERVER_HPP
#define SERVER_HPP

#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
using std::string;

struct Server {
    int sock;
    int clientSock;
    sockaddr_in address;
    unsigned short port = 9090;
    bool running = false;

    bool buildSocket(Server& s);
    bool acceptClient(Server& s);
    void handleClient(Server& s);
};

#endif