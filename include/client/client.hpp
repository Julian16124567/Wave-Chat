#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <gpgme.h>
using std::string;

struct Client {
    
    //attributes
    //p to p connection
    int sock;
    string connto;
    sockaddr_in server;

    //gpg key
    string email;
    string pubkey;
    gpgme_data_t dh;
    unsigned int key;

    //serverconnection
    int sockServer;
    sockaddr_in serverServer;
    string serverIP = "172.20.10.3";
    string data;

    //functions
    void buildSocket(Client& c);
    void buildSocketServer(Client& c); 
    void sendToServer(Client& c);

    Client() : sock(-1), sockServer(-1) {}
    ~Client() {
        if (sock >= 0) close(sock);
    }
};

#endif

