#include <iostream>
#include <string>
#include <thread>
#include <mutex>
#include <vector>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "../../include/server/server.hpp"

class PeerServer {
private:
    int serverSock = -1;
    bool running = false;
    std::vector<int> clientSockets;
    std::mutex socketsMutex;

public:
    ~PeerServer() {
        stop();
    }

    bool start(int port) {
        serverSock = socket(AF_INET, SOCK_STREAM, 0);
        if (serverSock < 0) {
            std::cerr << "[*] Failed to create socket" << std::endl;
            return false;
        }

        sockaddr_in addr{};
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = INADDR_ANY;
        addr.sin_port = htons(port);

        if (bind(serverSock, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
            std::cerr << "[*] Failed to bind to port " << port << std::endl;
            close(serverSock);
            return false;
        }

        if (listen(serverSock, 5) < 0) {
            std::cerr << "[*] Listen failed" << std::endl;
            close(serverSock);
            return false;
        }

        running = true;
        std::thread([this]() {
            acceptLoop();
        }).detach();

        std::cout << "[*] P2P Server listening on port " << port << std::endl;
        return true;
    }

    void stop() {
        running = false;
        if (serverSock >= 0) {
            close(serverSock);
            serverSock = -1;
        }
        
        std::lock_guard<std::mutex> lock(socketsMutex);
        for (int sock : clientSockets) {
            if (sock >= 0) close(sock);
        }
        clientSockets.clear();
    }

    void broadcast(const std::string& message) {
        std::lock_guard<std::mutex> lock(socketsMutex);
        auto it = clientSockets.begin();
        while (it != clientSockets.end()) {
            if (send(*it, message.c_str(), message.length(), 0) < 0) {
                close(*it);
                it = clientSockets.erase(it);
            } else {
                ++it;
            }
        }
    }

private:
    void acceptLoop() {
        while (running) {
            sockaddr_in peerAddr{};
            socklen_t addrLen = sizeof(peerAddr);
            
            int clientSock = accept(serverSock, (struct sockaddr*)&peerAddr, &addrLen);
            if (clientSock < 0) {
                if (running) {
                    std::cerr << "[*] Accept failed" << std::endl;
                }
                continue;
            }

            std::thread([this, clientSock, peerAddr]() {
                handlePeer(clientSock, peerAddr);
            }).detach();

            std::lock_guard<std::mutex> lock(socketsMutex);
            clientSockets.push_back(clientSock);
        }
    }

    void handlePeer(int sock, sockaddr_in peerAddr) {
        char peerIP[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &peerAddr.sin_addr, peerIP, INET_ADDRSTRLEN);
        std::cout << "[*] Peer connected from " << peerIP << std::endl;

        char buf[4096];
        while (running) {
            ssize_t bytesRead = recv(sock, buf, sizeof(buf)-1, 0);
            if (bytesRead <= 0) break;

            buf[bytesRead] = '\0';
            std::string message = std::string(peerIP) + ": " + buf;
            broadcast(message);
        }

        std::lock_guard<std::mutex> lock(socketsMutex);
        auto it = std::find(clientSockets.begin(), clientSockets.end(), sock);
        if (it != clientSockets.end()) {
            clientSockets.erase(it);
        }
        close(sock);
        std::cout << "[*] Peer " << peerIP << " disconnected" << std::endl;
    }
};
