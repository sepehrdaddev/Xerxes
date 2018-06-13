#ifndef XERXES_SERVERSTRUCTS_H
#define XERXES_SERVERSTRUCTS_H

#include <openssl/ssl.h>

struct configuration{
    int clients;
    int port;
    const char* cert_file;
    const char* key_file;
    int maxClients = 250;
    bool running;
    int socket;
    SSL_CTX *ctx;
};

struct Client {
    int id;
    int socket;
    SSL *ssl;
    virtual ~Client(){
        SSL_free(ssl);
        close(socket);
    }

};

#endif //XERXES_SERVERSTRUCTS_H
