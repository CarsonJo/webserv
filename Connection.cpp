#include "Connection.hpp"

Connection::Connection() : arr(BASE_ALLOC), nfds(0)
{

}

Connection::~Connection()
{

}

struct pollfd	*Connection::c_arr()
{
	return (&arr[0]);
}

int	Connection::get_size() const
{
	return (nfds);
}

void	Connection::add(int fd, int flag)
{
	if (nfds + 1 >= arr.size())
		arr.push_back((struct pollfd){.fd = fd, .events = flag, .revents = 0});
	else
		arr[nfds] = (struct pollfd){.fd = fd, .events = flag, .revents = 0};
	nfds++;
}

void	Connection::erase(unsigned long int index)
{
	if (index >= nfds)
		return ;
	close(arr[index].fd);
	if (nfds > 0)
		arr[index] = arr[nfds - 1];
	nfds--;
}
#include <iostream>
void	Connection::new_connect(Addrinfo& info)
{
	int			fd;
	socklen_t	recv = sizeof(*info.get_addr());

	if (nfds >= 0 && arr[0].revents == POLLIN)
	{
		fd = accept(arr[0].fd, info.get_addr(), &recv);
		this->add(fd, POLLIN);
	}
}