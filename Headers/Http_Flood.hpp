#ifndef XERXES_HTTP_FLOOD_H
#define XERXES_HTTP_FLOOD_H

#include <openssl/ssl.h>

#include "Configuration.hpp"
#include "Logger.hpp"
#include "Randomizer.hpp"
#include "Attack_Vector.hpp"
#include "httphdr.hpp"

class Http_Flood : public Attack_Vector {
    friend class Slowloris;
    friend class Null_Flood;
public:
    Http_Flood () = default;
    explicit Http_Flood (std::shared_ptr<Config> conf);
    void run() override;

private:
    void attack(const int *id) override;
    virtual void attack_ssl(const int *id);
    virtual int make_socket(const char *host, const char *port, int sock_type);
    SSL_CTX* InitCTX();
    SSL *Apply_SSL(int socket, SSL_CTX *ctx);
    void cleanup(const int *socket);
    void cleanup(SSL *ssl, const int *socket, SSL_CTX *ctx);
    void read_socket(int socket);
    void read_socket(SSL *ssl);
    int write_socket(int socket, const char* string, int length);
    int write_socket(SSL *ssl, const char* string, int length);
    const SSL_METHOD *GetMethod();
    virtual void init_header(std::string& header);
    virtual void init_header(std::string& header, bool);
};


#endif //XERXES_HTTP_FLOOD_H
