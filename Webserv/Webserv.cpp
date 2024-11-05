#include "Webserv.hpp"

Webserv::Webserv() : arr(0), original_serv(0), virtualserv(0), master_socket(0)
{

}

Webserv::~Webserv()
{
	for (unsigned long i = 0; i < virtualserv.size(); i++)
		delete virtualserv[i];
}

struct pollfd	*Webserv::c_arr()
{
	return (&arr[0]);
}

struct pollfd	*Webserv::connect_arr()
{
	if (static_cast<unsigned long>(master_socket) < arr.size())
		return (&arr[master_socket]);
	return (0);
}

int	Webserv::get_size() const
{
	return (arr.size());
}

int	Webserv::get_connect_size() const
{
	return (arr.size() - master_socket);
}

void	Webserv::add_connect(int fd, int flag, int master_socket)
{
	arr.push_back((struct pollfd){.fd = fd, .events = flag, .revents = 0});
	original_serv.push_back(master_socket);
	virtualserv[master_socket]->add_connection(&arr[arr.size() - 1]);
}

void	Webserv::add_serv(VirtualServ* virtserv)
{
	virtualserv.push_back(virtserv);
}

void	Webserv::add_master(const VirtualServ& virt_serv)
{
	arr.push_back((struct pollfd){.fd = virt_serv.get_fd(), .events = POLLIN, .revents = 0});
	master_socket++;
}

void	Webserv::erase(int fd)
{
	if (fd < 0)
		throw (std::exception());
	close(fd);
	for (std::vector<struct pollfd>::iterator i = arr.begin(); i != arr.end(); i++)
	{
		if (i->fd == fd)
		{
			arr.erase(i);
			break ;
		}
	}
}

void	Webserv::new_connect(int &event)
{
	int			fd;

	for (int i = 0; i < master_socket; i++)
	{
		if (arr[i].revents == POLLIN)
		{
			std::cout << "add" << std::endl;
			fd = virtualserv[i]->accept_connect();
			if (fcntl(fd, F_SETFL, O_NONBLOCK) < 0)
				throw(std::exception());
			this->add_connect(fd, POLLIN | POLLHUP | POLLERR | POLLNVAL, i);
			event--;
		}
		else if (arr[i].revents > 0)
			throw(std::exception());
	}
}

void	Webserv::init_all()
{
	for (unsigned long i = 0; i < virtualserv.size(); i++)
		virtualserv[i]->launch_serv();
}

void	Webserv::handle_recv(int &event)
{
	for (unsigned long i = 0; i < virtualserv.size() && event > 0; i++)
		virtualserv[i]->handle_connection(event, *this);
}