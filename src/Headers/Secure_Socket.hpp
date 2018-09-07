#ifndef SECURE_SOCKET_H
#define SECURE_SOCKET_H

#include <openssl/ssl.h>

#include "Socket.hpp"

class Secure_Socket : public Socket {
public:
	Secure_Socket() = default;
	explicit Secure_Socket(Host host, Protocol proto);
	virtual void Write(std::string& string) override;
	virtual void Connect() override;
	virtual void Disconnect() override;
	virtual ~Secure_Socket() = default;
private:
	SSL *ssl = nullptr;
	SSL_CTX *ctx = nullptr;
	SSL_METHOD *method = nullptr;
	virtual void CreateSSL();
	virtual void CreateCTX();

};

#endif /* SECURE_SOCKET_H */
