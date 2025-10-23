#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <gpgme.h>
using std::string;

struct Client {
    
    //attributes
    int sock;
    string connto;
    sockaddr_in server;
    string email;
    string pubkey;
    gpgme_data_t keydata;
    gpgme_data_t dh;
    unsigned int key;

    //functions
    bool ipValid(const Client& c);
    bool buildSocket(Client& c);
};

#endif
