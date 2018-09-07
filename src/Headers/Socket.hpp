#ifndef SOCKET_H
#define SOCKET_H

#include <string>
#include <netdb.h>

class Socket {
	friend class Secure_Socket;
public:
	enum Protocol{TCP = IPPROTO_TCP, UDP = IPPROTO_UDP};
	struct Host{std::string addr{""}; std::string port{"80"};};

	Socket() = default;
	explicit Socket(Host host, Protocol proto = TCP);
	virtual void Connect();
	virtual void Write(std::string& string);
	virtual void Disconnect();
	virtual void SetHost(Host h);
	virtual void SetSocketType(Protocol proto);
	virtual ~Socket() = default;
private:
	int socket = 0;
	Protocol protocol = TCP;
	Host host{};
	virtual void create();

};

#endif /* SOCKET_H */
