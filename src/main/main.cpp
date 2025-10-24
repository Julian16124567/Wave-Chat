#include <iostream>
#include <ncurses.h>
#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <exception>
//#include <librats.h>
#include "../../include/server/server.hpp"
#include "../../include/client/client.hpp"
#include "../../include/tui/tui.hpp"
using std::string;

void connecPtoP(Client& c) {
    c.buildSocket(c);
    
}

void connectServer(Client& c) {

}

int main() {
    
    //connecting client with server
    Client c;
    



    //initialising start window
    Tui t;
    try {    
        t.WindowInit(t);
        t.ColorMain();
        t.drawWinone(t);
    } catch(const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
}