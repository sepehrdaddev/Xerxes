#include <unistd.h>
#include <cstring>

#include "../Headers/Secure_Socket.hpp"
#include "../Headers/Logging.hpp"

Secure_Socket::Secure_Socket(Host host, Protocol proto)
	: Socket(host, proto){
	SetSocketType(proto);
	Connect();
}

void Secure_Socket::CreateSSL(){
	if(!ctx){
		ssl = SSL_new(ctx);
		SSL_set_fd(ssl, sock);
		if(SSL_connect(ssl) == -1){
			print_error("Unable to connect using ssl");
			exit(EXIT_FAILURE);
		}
	}else{
		print_error("ctx is not initialized");
		exit(EXIT_FAILURE);
	}
}

void Secure_Socket::CreateCTX(){
	SSL_CTX *ctx;
	ctx = SSL_CTX_new(method);
	if(!ctx){
		print_error("Unable to connect using ssl");
		exit(EXIT_FAILURE);
	}
}

Secure_Socket::~Secure_Socket(){
	Disconnect();
}

void Secure_Socket::Write(const char *string){
	if(SSL_write(ssl, string, strlen(string)) == -1){
		Disconnect();
		Connect();
	}
}

void Secure_Socket::Disconnect(){
	if(sock){
		close(sock);
		sock = 0;
	}
	if(ssl)
		SSL_free(ssl);
	if(ctx)
		SSL_CTX_free(ctx);
}

void Secure_Socket::Connect(){
	if(!sock)
		create();
	CreateCTX();
	CreateSSL();
}

void Secure_Socket::SetSocketType(Protocol proto){
	protocol = proto;
	switch(proto){
  case TCP:
    method = TLSv1_1_client_method();
    break;
  case UDP:
    method = DTLSv1_2_client_method();
    break;
  default:break;
  }
}
