#ifndef XERXES_SERVER_H
#define XERXES_SERVER_H

#include <string>
#include <vector>
#include "ServerStructs.hpp"
#include "ServerThread.hpp"

class Server {

public:
    explicit Server(int port);
    virtual ~Server();
    void Serve();
private:
    static int make_socket(int& port);
    static int read_socket(SSL *sock, std::string *buff);
    static int write_socket(SSL *sock, std::string *buff);
    static EVP_PKEY * generate_key();
    X509 *generate_x509(EVP_PKEY * pkey);
    bool save_key(EVP_PKEY * pkey, X509 * x509);
    void generate_certificates();
    static void init_openssl();
    static void cleanup_openssl();
    static SSL_CTX *InitCTX();
    static void configure_CTX(SSL_CTX *ctx);
    static void *HandleClient(void *arg);
    static void *command(void *);
    static void *Accept_Clients(void *);
    void check_certs();
    static bool is_connected(SSL *ssl);
    static void SendToAll(std::string *buff);
    static void List_clients();
    static void remove_client(Client *client);

    static configuration *config;
    static std::vector<Client *> clients;
    static std::vector<ServerThread *> threads;


};


#endif //XERXES_SERVER_H
