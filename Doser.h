#ifndef XERXES_DOSER_H
#define XERXES_DOSER_H


#include <array>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include "Configuration.h"
#include "Logger.h"

class Doser {
public:
    Doser(config *conf, Logger *logger);
    void run();

private:
    int make_socket(const char *host, const char *port);
    void read_socket(int socket);
    void read_socket(SSL *ssl);
    int write_socket(int socket, const char* string, int length);
    int write_socket(SSL *ssl, const char* string, int length);
    std::string craft_packet(bool keep_alive=false);
    static void broke(int);
    std::string createStr();
    int randomInt(int min, int max);
    void attack(const int *id);
    void attack_icmp(const int *id);
    std::string randomizeUserAgent();
    SSL_CTX* InitCTX();
    SSL *Apply_SSL(int socket, SSL_CTX *ctx);
    void cleanup(const int *socket);
    void cleanup(SSL *ssl, const int *socket, SSL_CTX *ctx);
    config *conf;
    Logger *logger;
    std::vector<std::string> encoding{"\'\'", "*", "identity", "gzip", "deflate"};
    std::vector<std::string> caching{"no-cache", "max-age=0"};
    std::vector<std::string> charset{"ISO-8859-1", "utf-8", "Windows-1251", "ISO-8859-2", "ISO-8859-15"};
    std::vector<std::string> contenttype{"multipart/form-data", "application/x-url-encoded"};
    std::vector<std::string> methods{"GET", "POST", "HEAD"};
    std::vector<std::string> referer{"https://www.google.com/", "https://www.yahoo.com/", "https://www.bing.com/",
                                     "https://twitter.com/", "https://www.facebook.com/", "https://www.msn.com/",
                                     "https://www.youtube.com/", "https://yandex.com/", "https://www.amazon.com/"};

};


#endif //XERXES_DOSER_H
