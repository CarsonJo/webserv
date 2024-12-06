#include "Fsocket.hpp"


Fsocket::Fsocket() : fd(-1), launched(0), my_info()
{

}

Fsocket::Fsocket(const Addrinfo& info) : launched(0), my_info(info)
{
	fd = socket(my_info.get_family(), my_info.get_socktype() , my_info.get_protocol());
	if (fd < 0)
		throw(std::exception());
	if (fcntl(fd, F_SETFL, O_NONBLOCK) < 0)
		throw(std::exception());
}

Fsocket::Fsocket(const Fsocket& to_copy) : fd(to_copy.fd), launched(to_copy.launched), my_info(to_copy.my_info)
{

}

Fsocket::~Fsocket()
{
	if (fd >= 0)
		close(fd);
}

int	Fsocket::accept_connect()
{
	if (!launched)
		throw(std::exception());
	int			ret;
	socklen_t	sock = my_info.get_addrlen();

	ret = accept(fd, my_info.get_addr(), &sock);
	return (ret);
}

void	Fsocket::set_fd(int temp)
{
	fd = temp;
}

void	Fsocket::set_launched(bool val)
{
	launched = val;
}

int	Fsocket::get_fd() const
{
	return (fd);
}

void Fsocket::operator=(const Fsocket& to_copy)
{
	if (fd >= 0)
		close(fd);
	fd = to_copy.fd;
	my_info = to_copy.my_info;
}
