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
        gpgme_data_new(&c.keydata);

        gpgme_op_export(ctx, c.email.c_str(), c.key, 0);
        gpgme_key_t key;
        gpgme_op_export_keys(ctx, &key, 0, c.keydata);

        char buf[4096];
        gpgme_data_seek(c.keydata, 0, SEEK_SET);
        while (gpgme_data_read(c.keydata, buf, sizeof(buf)) > 0) {
            c.pubkey += buf;
        }
        gpgme_data_release(c.keydata);
    }

    void importPublicKey(Client& c) {
        gpgme_data_new_from_mem(&c.dh, &c.keydata.c_str(), c.keydata.length(), 0);
        
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