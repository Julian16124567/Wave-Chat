#include <iostream>
#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <exception>
#include "../../include/client/client.hpp"
using std::string, std::runtime_error;

//build socket
bool buildSocket(Client& c) {
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

//check if ip ist valid
//will be used in other files
bool ipValid(const Client& c) {
    struct sockaddr_in sa;
    int result = inet_pton(AF_INET, c.connto.c_str(), &(sa.sin_addr));
    return result != 0;
}