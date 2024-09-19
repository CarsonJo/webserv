#include "FSocket.hpp"

FSocket::FSocket(const Addrinfo& info)
{
	fd = socket(info.get_family(), info.get_socktype() , info.get_protocol());
	if (fd < 0)
		throw(std::exception());
	if (fcntl(fd, F_SETFL, O_NONBLOCK) < 0)
		throw(std::exception());
}

FSocket::FSocket(int fd)
{
	this->fd = fd;
}

FSocket::~FSocket()
{
	close(fd);
}

int	FSocket::get_fd()
{
	return (fd);
}
