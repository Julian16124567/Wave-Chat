#ifndef SOCK_HPP
#define SOCK_HPP

#include <string>
#include <sys/socket.h>   
#include <arpa/inet.h>   
#include <unistd.h>   
#include <iostream>
using std::string;

struct SockData
{
    unsigned char key[crypto_secretbox_KEYBYTES];     
    unsigned char nonce[crypto_secretbox_NONCEBYTES];  
    unsigned char* message = nullptr;      
    unsigned char* enc_message = nullptr; 
};

#endif
