#include "Webserv.hpp"

Webserv::Webserv() : arr(0), virtualserv(), master_socket(0)
{

}

Webserv::~Webserv()
{
	unsigned long i = 0;

	for (i = 0; i < virtualserv.size(); i++)
		delete virtualserv[i];
	for (i = 0; i < arr.size(); i++)
		close(arr[i].fd);
	for (std::map<int, Request*>::iterator it = request.begin(); it != request.end(); it++)
		delete it->second;
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

void	Webserv::add_connect(int fd, int flag, VirtualServ *link)
{
	arr.push_back((struct pollfd){.fd = fd, .events = flag, .revents = 0});
	linkServ.insert(std::pair<int, VirtualServ*>(fd, link));
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
			this->add_connect(fd, POLLIN | POLLHUP | POLLERR | POLLNVAL, virtualserv[i]);
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
	for (unsigned long i = master_socket; i < arr.size() && event > 0; i++)
	{
		if ((arr[i].revents & POLLIN) != 0)
		{
			try
			{
				request.insert(std::pair<int, Request *>(arr[i].fd, Request::parsedRequest(arr[i].fd)));
				arr[i].events = POLLOUT;
				event--;
			}
			catch(std::exception &a)
			{
				std::cout<<"removed" << std::endl;
				std::map<int, Request*>::iterator	it = request.find(arr[i].fd); //ne devrait jamais trouver
				if (it != request.end())
					request.erase(request.find(arr[i].fd));//ne devrait jamais s'executer
				close(arr[i].fd);
				arr.erase(arr.begin() + i);
				event--;
			}
		}
		else if ((arr[i].revents & POLLOUT) != 0)
		{
			std::map<int, Request*>::iterator	it = request.find(arr[i].fd);
			VirtualServ* point = linkServ.at(arr[i].fd);
			if (it == request.end())
				throw(std::exception());//exception qui ne devrais jamais se declencher;
			it->second->response(arr[i].fd, point);
			delete it->second;
			request.erase(it);
			arr[i].events = POLLIN;
			event--;
		}
	}
}