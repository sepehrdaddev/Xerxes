#ifndef SOCKET_H
#define SOCKET_H

#include <string>
#include <netdb.h>

class Socket {
	friend class Secure_Socket;
public:
	enum Protocol{TCP = SOCK_STREAM, UDP = SOCK_DGRAM};
	struct Host{std::string addr{""}; std::string port{"80"};};

	Socket() = default;
	explicit Socket(Host host, Protocol proto = TCP);
	virtual void Connect();
	virtual void Write(const char *string);
	virtual void Disconnect();
	virtual void SetHost(Host h);
	virtual void SetSocketType(Protocol proto);
	virtual ~Socket();
private:
	int sock = 0;
	Protocol protocol = TCP;
	Host host{};
	virtual void create();

};

#endif /* SOCKET_H */
