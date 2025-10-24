#include <iostream>
#include <string>
#include <gpgme.h>
#include "../../include/client/client.hpp"
using std::string;


//gpg key class
class GPGKeyMngr {
private:
    gpgme_ctx_t ctx;

public:
    GPGKeyMngr(Client& c) {
        gpgme_check_version(0);
        gpgme_error_t err = gpgme_new(&ctx);
        if (err) {
            throw std::runtime_error("[*] Failed to initialize GPGME!");
        }
    }
    ~GPGKeyMngr() {
        if (ctx) gpgme_release(ctx);
    }

    void exportPublicKey(Client& c) {
    gpgme_data_t keydata;
    gpgme_data_new(&keydata);

    gpgme_error_t err = gpgme_op_export(ctx, c.email.c_str(), 0, keydata);
    if (err) {
        throw std::runtime_error("[*] Failed to export public key!");
    }

    char buf[4096];
    ssize_t n;
    while ((n = gpgme_data_read(keydata, buf, sizeof(buf))) > 0) {
        c.pubkey.append(buf, n);
    }

    gpgme_data_release(keydata);
    }


    void importPublicKey(Client& c) {
        gpgme_data_t keydata;

        gpgme_data_new_from_mem(&c.dh, keydata.c_str(), keydata.length(), 0);
        
        gpgme_error_t err = gpgme_op_import(ctx, c.dh);
        if (err) {
            throw std::runtime_error("[*] Failed to import public key!");
        }
        gpgme_data_release(c.dh);
    }

    bool echangeKeys(Client& c) {
        try{
            GPGKeyMngr keyManager;
        }
    }
};

//clear gpg initializers
void clearGpg(Client& c) {
    c.email = "";
    c.pubkey = "";
    gpgme_data_t dh;
}

//check if key valid and exchnage complete
bool keyValid(const Client& c) {
    
}