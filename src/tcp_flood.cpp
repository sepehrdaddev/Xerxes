#include "tcp_flood.h"
#include <arpa/inet.h>


tcp_flood::tcp_flood(std::shared_ptr<Config> config) : base_flood(std::move(config), SOCK_STREAM){

}
