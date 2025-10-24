#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdexcept>
#include "../../include/client/client.hpp"
using std::string;

//build socket
void buildSocketServer(Client& c) {
    if (c.serverIP.empty()) {
        std::cout << "[*] No Server ip provided!" << std::endl;
    }
    c.sock = socket(AF_INET, SOCK_STREAM, 0);
    if (c.sock < 0) {
        throw std::runtime_error("[*] Socket Creation Failed!");
    }
    sockaddr_in address;
    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    if (inet_pton(AF_INET, c.connto.c_str(), &c.server.sin_addr) <= 0) {
        throw std::runtime_error("[*] Invalid IP address format!");
    }

    if (connect(c.sock, (struct sockaddr*)&c.server, sizeof(c.server)) < 0) {
        close(c.sock);
        throw std::runtime_error("[*] Failed to connect to server!");
    }

    std::cout << "[*] Connected to server." << std::endl;
}

//send data to server
//string messge is attribute for message
void sendToServer(Client& c, string message) {
    c.data = message;
    ssize_t bytesSent = send(c.sock, c.data.c_str(), c.data.length(), 0);
    if (bytesSent < 0) {
        throw std::runtime_error("[*] Failed to send data to server!");
    }
}

void registerWithServer(Client& c, int localListenPort) {
    buildSocketServer(c); 
    std::string msg = "REGISTER " + c.connto + " " + std::to_string(localListenPort) + "\n";
    send(c.sock, msg.c_str(), msg.size(), 0);
    
    char buf[256]; ssize_t r = recv(c.sock, buf, sizeof(buf)-1, 0);
    if (r > 0) buf[r]=0;
    
    close(c.sock); c.sock = -1;
}

std::vector<string> requestPeers(Client& c) {
    buildSocketServer(c);
    std::string req = "PEERS\n";
    send(c.sock, req.c_str(), req.size(), 0);

    std::string resp;
    char buf[4096];
    ssize_t r;
    while ((r = recv(c.sock, buf, sizeof(buf), 0)) > 0) {
        resp.append(buf, r);
        if (r < (ssize_t)sizeof(buf)) break;
    }
    close(c.sock); c.sock = -1;

    std::vector<string> list;
    std::istringstream iss(resp);
    string line;
    while (std::getline(iss, line)) {
        if (!line.empty()) list.push_back(line);
    }
    return list;
}