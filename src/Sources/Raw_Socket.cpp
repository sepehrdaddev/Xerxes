#include <arpa/inet.h>
#include <unistd.h>

#include "../Headers/Logging.hpp"
#include "../Headers/Raw_Socket.hpp"

Raw_Socket::Raw_Socket() {

}

Raw_Socket::~Raw_Socket() {
}


unsigned short Raw_Socket::checksum(unsigned short *buf, int len){
	static unsigned long sum;
	for(sum = 0;len > 0;len--)
		sum += *buf++;
	sum = (sum >> 16) + (sum &0xffff);
	sum += (sum >> 16);
	return std::move((unsigned short)(~sum));
}

void Raw_Socket::Create(){
	if(!sock)
		create();
}

void Raw_Socket::Send(const char *buffer){
  dst->sin_addr.s_addr = iph->daddr;
  dst->sin_family = AF_UNSPEC;
  if((static_cast<int>(sendto(sock, buffer, iph->tot_len, 0, reinterpret_cast<sockaddr *>(&dst), sizeof(dst)))) == -1){
		Close();
		Create();
	}
}

void Raw_Socket::create(){
	int on = 1;
	if((sock = socket(AF_INET, SOCK_RAW, protocol)) == -1){
		print_error("socket() error");
		exit(EXIT_FAILURE);
	}

	if(setsockopt(sock, IPPROTO_IP, IP_HDRINCL, &on, sizeof(on)) == -1){
		print_error("setsockopt() error");
		exit(EXIT_FAILURE);
	}

	if(setsockopt(sock, SOL_SOCKET, SO_BROADCAST, reinterpret_cast<const char*>(&on), sizeof(on)) == -1){
		print_error("setsockopt() error");
		exit(EXIT_FAILURE);
	}

}

void Raw_Socket::Close(){
	if(sock){
		close(sock);
		sock = 0;
	}
}

void Raw_Socket::SetDst(struct sockaddr_in *d){
	dst = d;
}

void Raw_Socket::SetIphdr(struct iphdr *ip){
	iph = ip;
}
