#include "Webserv.hpp"

Webserv::Webserv() : arr(0), virtualserv(), master_socket(0)
{

}

Webserv::~Webserv()
{
	unsigned long i = 0;

	// for (i = 0; i < virtualserv.size(); i++)
	// 	delete virtualserv[i];
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

void	Webserv::add_connect(int fd, int flag, ServerBlock *link)
{
	arr.push_back((struct pollfd){.fd = fd, .events = flag, .revents = 0});
	linkServ.insert(std::pair<int, ServerBlock*>(fd, link));
}

void	Webserv::add_serv(std::vector<ServerBlock*>& virtserv)
{
	virtualserv = virtserv;
	for (unsigned long i = 0; i < virtserv.size(); i++)
		arr.push_back((struct pollfd){.fd = virtserv[i]->get_fd(), .events = POLLIN | POLLHUP | POLLERR | POLLNVAL, .revents= 0});
	master_socket = virtserv.size();
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
				request.insert(std::pair<int, Request *>(arr[i].fd, Request::parsedRequest(arr[i].fd, linkServ.at(arr[i].fd))));
				arr[i].events = POLLOUT | POLLIN | POLLHUP | POLLERR | POLLNVAL;
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
			if (it == request.end())
				throw(std::exception());//exception qui ne devrais jamais se declencher;
			if (int error = it->second->response(arr[i].fd))
			{
				arr[i].events = POLLIN | POLLHUP | POLLERR | POLLNVAL;
				delete it->second;
				request.erase(it);
				event--;
				if (error == CLOSE)
					this->erase(arr[i].fd);
			}
			event--;
		}
	}
}