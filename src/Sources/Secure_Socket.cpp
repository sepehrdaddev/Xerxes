#include <unistd.h>

#include "../Headers/Secure_Socket.hpp"
#include "../Headers/Logging.hpp"

Secure_Socket::Secure_Socket(Host host, Protocol proto)
	: Socket(host, proto){
	Connect();
}

void Secure_Socket::CreateSSL(){
	if(!ctx){
		ssl = SSL_new(ctx);
		SSL_set_fd(ssl, socket);
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

void Secure_Socket::Write(std::string& string){
	if(SSL_write(ssl, string.c_str(), string.length()) == -1){
		Disconnect();
		Connect();
	}
}

void Secure_Socket::Disconnect(){
	if(socket){
		close(socket);
		socket = 0;
	}
	if(ssl)
		SSL_free(ssl);
	if(ctx)
		SSL_CTX_free(ctx);
}

void Secure_Socket::Connect(){
	if(!socket)
		create();
	CreateCTX();
	CreateSSL();
}
