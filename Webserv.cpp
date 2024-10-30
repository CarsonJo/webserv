#include "Webserv.hpp"

Webserv::Webserv() : arr(0), original_serv(0), virtualserv(0), master_socket(0)
{

}

Webserv::~Webserv()
{

}

struct pollfd	*Webserv::c_arr()
{
	return (&arr[0]);
}

int	Webserv::get_size() const
{
	return (arr.size());
}

void	Webserv::add_connect(int fd, int flag, int master_socket)
{
	arr.push_back((struct pollfd){.fd = fd, .events = flag, .revents = 0});
	original_serv.push_back(master_socket);
	virtualserv[master_socket].add_connection(&arr[arr.size() - 1]);
}

void	Webserv::add_serv(const VirtualServ& virtserv)
{
	virtualserv.push_back(virtserv);
}

void	Webserv::add_master(const VirtualServ& virt_serv)
{
	arr.push_back((struct pollfd){.fd = virt_serv.get_fd(), .events = POLLIN, .revents = 0});
	master_socket++;
}

void	Webserv::erase(unsigned long int index)
{
	if (index >= arr.size())
		return ;
	close(arr[index].fd);
	virtualserv[original_serv[index]].remove_connection(&arr[index]);
	arr.erase(arr.begin() + index);
	original_serv.erase(original_serv.begin() + index);
}

void	Webserv::new_connect()
{
	int			fd;

	for (int i = 0; i < master_socket; i++)
	{
		if (arr[i].revents == POLLIN)
		{
			fd = virtualserv[i].accept_connect();
			if (fcntl(fd, F_SETFL, O_NONBLOCK) < 0)
				throw(std::exception());
			this->add_connect(fd, DISCUSS, i);
		}
	}
}