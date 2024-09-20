#include "Addrinfo.hpp"

Addrinfo::Addrinfo(int family, int socktype, int protocol, int flags, const std::string& port)
{
	hints.ai_family = family;
	hints.ai_socktype = socktype;
	hints.ai_protocol = protocol;
	hints.ai_flags = flags;
	hints.ai_addr = 0;
	hints.ai_addrlen = 0;
	hints.ai_next = 0;
	getaddrinfo(0, port.c_str(), &hints, &res);
	this->port = port;
}

Addrinfo::~Addrinfo()
{
	free(res);
}

int	Addrinfo::get_family() const
{
	return (hints.ai_family);
}

int	Addrinfo::get_flags() const
{
	return (hints.ai_flags);
}

int	Addrinfo::get_protocol() const
{
	return (hints.ai_protocol);
}

int	Addrinfo::get_socktype() const
{
	return (hints.ai_socktype);
}

std::string	Addrinfo::get_port() const
{
	return (port);
}

socklen_t	Addrinfo::get_addrlen() const
{
	return (res->ai_addrlen);
}

struct sockaddr*	Addrinfo::get_addr()
{
	return (res->ai_addr);
}
