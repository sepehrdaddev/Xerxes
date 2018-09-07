#include <netdb.h>
#include <unistd.h>
#include <cstring>

#include "Socket.hpp"
#include "../Headers/Logging.hpp"


Socket::Socket(Host host, Protocol proto) : host{host}, protocol{proto}{
	Connect();
}

Socket::~Socket(){
	Disconnect();
}

void Socket::Disconnect(){
	if(socket){
		close(socket);
		socket = 0;
	}
}

void Socket::Connect(){
	if(!socket)
		create();
}

void Socket::create(){
	struct addrinfo hints{}, *servinfo, *p;
	int r;
	bzero(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = static_cast<int>(protocol);
	if((r = getaddrinfo(host.addr.c_str(), host.port.c_str(), &hints, &servinfo))!=0) {
		char message[512];
		snprintf(message, sizeof(message), "Getaddrinfo-> %s", gai_strerror(r));
		print_error(message);
		exit(EXIT_FAILURE);
	}
	for(p = servinfo; p != nullptr; p = p->ai_next) {
		if((socket = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
			continue;
		}
		if(connect(socket, p->ai_addr, p->ai_addrlen) == -1) {
			Disconnect();
			continue;
		}
		break;
	}
	if(servinfo)
		freeaddrinfo(servinfo);
	if(!p) {
		print_error("No connection could be made");
		exit(EXIT_FAILURE);
	}
}

void Socket::Write(std::string& string){
	if(write(socket, string.c_str(), string.length()) == -1){
		Disconnect();
		Connect();
	}
}

void Socket::SetHost(Host h){
	host = h;
	Disconnect();
	Connect();
}

void Socket::SetSocketType(Protocol p){
	protocol = p;
	Disconnect();
	Connect();
}
