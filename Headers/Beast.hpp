#ifndef XERXES_Beast_H
#define XERXES_Beast_H

#include <arpa/inet.h>

#include "Attack_Vector.hpp"

class Beast : public Attack_Vector {
public:
    explicit Beast(std::shared_ptr<Config> conf);

private:
    #define FL_PEER_WANT_NEXT_STATE		(0x04)
    #define MAX_PEERS		(999)
    #define TO_TCP_CONNECT		(10)

    // enums
    enum _states { UNKNOWN = 0, TCP_CONNECTING, SSL_CONNECTING, SSL_HANDSHAKING, SSL_DUMMYWRITE };

    // structs
    struct _statistics {
        uint32_t total_tcp_connections;
        uint32_t total_renegotiations;
        uint32_t total_ssl_connect;
        uint32_t error_count;
        uint64_t epoch_start_usec;
        uint32_t epoch_start_renegotiations;
    };

    struct _opt {
        uint16_t n_peers;
        uint16_t n_max_peers;
        fd_set rfds;
        fd_set wfds;
        int max_sox;
        SSL_CTX *ctx;
        struct _statistics stat;
    };
    struct _peer {
        uint32_t flags;
        SSL *ssl;
        int sox;
        enum _states state;
        struct sockaddr_in addr;
        uint32_t count_renegotiations;
        uint32_t tv_connect_sec;
    };
    struct _opt opt{};
    struct _peer peers[MAX_PEERS];

    // functions
    void SSL_set_rw(struct _peer *p, int ret);
    int ssl_handshake_io(struct _peer *p);
    int ssl_connect_io(struct _peer *p);
    int ssl_dummywrite_io(struct _peer *p);


    void PEER_SSL_dummywrite(struct _peer *p);
    void PEER_SSL_renegotiate(struct _peer *p);
    void PEER_SSL_connect(struct _peer *p);
    void PEER_connect(struct _peer *p);
    void PEER_disconnect(struct _peer *p);
    void PEER_read(struct _peer *p);
    void PEER_write(struct _peer *p);

    int tcp_connect_io(struct _peer *p);
    int tcp_connect_try_finish(struct _peer *p, int ret);
    int tcp_connect(struct _peer *p);

    void NextState(struct _peer *p);
    void CompleteState(struct _peer *p);
    void init();
    void update_stat(struct timeval *tv);
    uint64_t getusec(struct timeval *tv);
    void attack() override;
};


#endif //XERXES_Beast_H
