#include <iostream>
#include <string>
#include <thread>
#include <mutex>
#include <vector>
#include <sstream>
#include <cstring>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "../../include/server/server.hpp"

static std::mutex peers_m;
static std::vector<std::string> peers; 

bool Server::buildSocket() {
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) return false;

    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if (bind(sock, (struct sockaddr*)&address, sizeof(address)) < 0) {
        close(sock); sock = -1; return false;
    }
    if (listen(sock, 10) < 0) {
        close(sock); sock = -1; return false;
    }

    running = true;
    std::thread([this](){
        while (running) {
            acceptClient();
        }
    }).detach();

    std::cout << "[*] Server listening on port " << port << std::endl;
    return true;
}

bool Server::acceptClient() {
    sockaddr_in clientAddr{};
    socklen_t addrlen = sizeof(clientAddr);
    int fd = accept(sock, (struct sockaddr*)&clientAddr, &addrlen);
    if (fd < 0) return false;
    std::thread(&Server::handleClient, this, fd).detach();
    return true;
}

void Server::handleClient(int clientFd) {
    constexpr size_t BUF = 4096;
    char buf[BUF];
    std::string incoming;
    while (true) {
        ssize_t r = recv(clientFd, buf, BUF, 0);
        if (r <= 0) break;
        incoming.append(buf, r);
        // process lines
        size_t pos;
        while ((pos = incoming.find('\n')) != std::string::npos) {
            std::string line = incoming.substr(0, pos);
            incoming.erase(0, pos+1);
            // simple protocol:
            // REGISTER <ip> <port>
            // PEERS
            std::istringstream iss(line);
            std::string cmd;
            iss >> cmd;
            if (cmd == "REGISTER") {
                std::string ip; int port;
                iss >> ip >> port;
                if (!ip.empty() && port > 0) {
                    std::lock_guard<std::mutex> g(peers_m);
                    peers.push_back(ip + ":" + std::to_string(port));
                    std::string ok = "OK\n";
                    send(clientFd, ok.c_str(), ok.size(), 0);
                }
            } else if (cmd == "PEERS") {
                std::string out;
                {
                    std::lock_guard<std::mutex> g(peers_m);
                    for (auto &p : peers) { out += p + "\n"; }
                }
                send(clientFd, out.c_str(), out.size(), 0);
            } else {
                std::string bad = "ERR unknown\n";
                send(clientFd, bad.c_str(), bad.size(), 0);
            }
        }
    }
    close(clientFd);
}



