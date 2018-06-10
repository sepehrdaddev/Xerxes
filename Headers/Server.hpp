#ifndef XERXES_SERVER_H
#define XERXES_SERVER_H

#include <string>

class Server {

public:
    explicit Server(int port);
    virtual ~Server();
    void Serve();
private:
    static int make_socket(int& port);
    static void read_socket(SSL *sock, std::string *buff);
    static void write_socket(SSL *sock, std::string *buff);
    static EVP_PKEY * generate_key();
    X509 *generate_x509(EVP_PKEY * pkey);
    bool save_key(EVP_PKEY * pkey, X509 * x509);
    void generate_certificates();
    static void init_openssl();
    static void cleanup_openssl();
    static SSL_CTX *InitCTX();
    static void configure_CTX(SSL_CTX *ctx);

    void check_certs();

    struct configuration{
        int clients;
        int port;
        const char* cert_file;
        const char* key_file;
        bool running;
    };
    configuration *config;

};


#endif //XERXES_SERVER_H
