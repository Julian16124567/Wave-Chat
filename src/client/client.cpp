#include <iostream>
#include <string>
#include <thread>
#include <mutex>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <exception>
#include "../../include/client/client.hpp"
using std::string, std::runtime_error;

//build socket
void buildSocket(Client& c) {
    c.sock = socket(AF_INET, SOCK_STREAM, 0);
    if (c.sock < 0) {
        throw runtime_error("[*] Socket Creation Failed!");
    }
    
    c.server.sin_family = AF_INET;
    c.server.sin_port = htons(9090);

    if (inet_pton(AF_INET, c.connto.c_str(), &c.server.sin_addr) <= 0) {
        throw runtime_error("[*] Invalid Ip addess format!");
    } else {
        std::cout << "[*] Socket Build." << std::endl;
    } 
}

//peer receiver
static void peerReceiver(int sock) {
    char buf[4096];
    while (true) {
        ssize_t r = recv(sock, buf, sizeof(buf)-1, 0);
        if (r <= 0) break;
        buf[r]=0;
        std::cout << "[peer] " << buf << std::endl;
    }
    close(sock);
}

bool connectToPeer(const string& ip, int port, int &outSock) {
    int s = socket(AF_INET, SOCK_STREAM, 0);
    if (s < 0) return false;
    sockaddr_in peer{};
    peer.sin_family = AF_INET;
    peer.sin_port = htons(port);
    if (inet_pton(AF_INET, ip.c_str(), &peer.sin_addr) <= 0) {
        close(s); return false;
    }
    if (connect(s, (struct sockaddr*)&peer, sizeof(peer)) < 0) {
        close(s); return false;
    }
    outSock = s;
    std::thread(peerReceiver, s).detach();
    return true;
}