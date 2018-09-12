#include <netdb.h>
#include <unistd.h>
#include <cstring>

#include "../Headers/Socket.hpp"
#include "../Headers/Logging.hpp"


Socket::Socket(Host host, Protocol proto){
	SetHost(host);
	SetSocketType(proto);
	Connect();
}

Socket::~Socket(){
	Disconnect();
}

void Socket::Disconnect(){
	if(sock){
		close(sock);
		sock = 0;
	}
}

void Socket::Connect(){
	if(!sock)
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
		if((sock= socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
			continue;
		}
		if(connect(sock, p->ai_addr, p->ai_addrlen) == -1) {
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

void Socket::Write(const char *string){
	if(write(sock, string, strlen(string)) == -1){
		Disconnect();
		Connect();
	}
}

void Socket::SetHost(Host h){
	host = h;
}

void Socket::SetSocketType(Protocol p){
	protocol = p;
}
