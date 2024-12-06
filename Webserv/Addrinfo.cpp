#include "Addrinfo.hpp"
#include <iostream>


Addrinfo::Addrinfo(int family, int socktype, int protocol, int flags, const std::string& port, const char *ip)
{
	hints.ai_family = family;
	hints.ai_socktype = socktype;
	hints.ai_protocol = protocol;
	hints.ai_flags = flags;
	hints.ai_addr = 0;
	hints.ai_addrlen = 0;
	hints.ai_next = 0;
	if (int a = getaddrinfo(ip, port.c_str(), &hints, &res))
	{
		std::cout << "addrinfo error: " << a << std::endl;
		std::exit(2);
	}
	this->ip = ip;
	this->port = port;
}

Addrinfo::Addrinfo() : hints((struct addrinfo){0,0,0,0,0,0,0,0}), res(0), port("-1")
{

}

Addrinfo::~Addrinfo()
{
	if (res != 0)
		freeaddrinfo(res);
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

Addrinfo::Addrinfo(const Addrinfo& to_copy) : hints(to_copy.hints), port(to_copy.port), ip(to_copy.ip)
{
	if (int a = getaddrinfo(ip.c_str(), port.c_str(), &hints, &res))
	{
		std::cout << "addrinfo error: " << a << std::endl;
		std::exit(2);
	}
}

void	Addrinfo::operator=(const Addrinfo& to_copy)
{
	if (res != 0)
		freeaddrinfo(res);
	hints = to_copy.hints;
	port = to_copy.port;
	ip = to_copy.ip;
	if (int a = getaddrinfo(ip.c_str(), to_copy.port.c_str(), &to_copy.hints, &res))
	{
		std::cout << "addrinfo error: " << a << std::endl;
		std::exit(2);
	}
}
