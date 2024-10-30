#include "Fsocket.hpp"

Fsocket::Fsocket(const Addrinfo& info) : my_info(info)
{
	fd = socket(info.get_family(), info.get_socktype() , info.get_protocol());
	if (fd < 0)
		throw(std::exception());
	if (fcntl(fd, F_SETFL, O_NONBLOCK) < 0)
		throw(std::exception());
}

Fsocket::Fsocket(const Fsocket& to_copy) : fd(to_copy.fd), my_info(to_copy.my_info)
{

}

Fsocket::~Fsocket()
{
	close(fd);
}

int	Fsocket::accept_connect()
{
	int			ret;
	socklen_t	sock = my_info.get_addrlen();

	ret = accept(fd, my_info.get_addr(), &sock);
	return (ret);
}

int	Fsocket::get_fd() const
{
	return (fd);
}

void Fsocket::operator=(const Fsocket& to_copy)
{
	fd = to_copy.fd;
	my_info = to_copy.my_info;
}
