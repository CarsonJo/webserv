# include "VirtualServ.hpp"
#include <iostream>
std::map<std::string, ParseFunction> init_static_elem();

std::map<std::string, ParseFunction> VirtualServ::server_elem = init_static_elem();

std::string	get_value(const std::string& line, int (*f)(int))
{
	int			j = 0;
	std::string	sub;
	std::size_t	pos = line.find_first_not_of(" \t");
	std::size_t	temp = pos;

	if (pos == std::string::npos)
		throw(std::exception());
	while (f(line[pos + j]) && pos + j < line.size())
		j++;
	if (pos + j >= line.size())
		throw(std::exception());
	sub = line.substr(pos + j);
	pos = sub.find_first_not_of(" \t");
	if (pos == std::string::npos || sub[pos] != ';')
		throw(std::exception());
	return (line.substr(temp, j));
}

static int myalnum(int c)
{
	if (c != '}' && isalnum(c))
		return (1);
	return (0);
}

void	server_var(const std::string &line, VirtualServ &serv)
{
	//mettre unb name par defaut au cas ou server name apparait deux fois dans le fichier de config
	std::string	temp = get_value(line, myalnum);
	serv.set_name(temp);
}

void	listen_var(const std::string &line, VirtualServ &serv)
{
	std::string	temp = get_value(line, isdigit);
	serv.set_port(temp);
}

std::map<std::string, ParseFunction> init_static_elem()
{
	std::map<std::string, ParseFunction> ret;

	ret["server_name"] = server_var;
	ret["listen"] = listen_var;
	return (ret);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

VirtualServ::VirtualServ() : socket_fd(), connection_fd(0), port(""), server_name("")
{

}

VirtualServ::VirtualServ(const VirtualServ& to_copy) : socket_fd(to_copy.socket_fd), connection_fd(to_copy.connection_fd), port(to_copy.port), server_name(to_copy.server_name)
{

}

VirtualServ::~VirtualServ()
{

}

// void	VirtualServ::operator=(VirtualServ& to_copy)
// {
// 	socket_fd = to_copy.socket_fd;
// 	connection_fd = to_copy.connection_fd;
// 	port = to_copy.port;
// 	server_name = to_copy.server_name;
// }

//verifier ce qui se passe si fail de construction de socket_fd
VirtualServ::VirtualServ(const Addrinfo& info) : socket_fd(info), connection_fd(0)
{

}

int	VirtualServ::accept_connect()
{
	return(socket_fd.accept_connect());
}

void	VirtualServ::add_connection(struct pollfd* fd)
{
	connection_fd.push_back(new Http_connection(fd, 0));
}

void	VirtualServ::remove_connection(struct pollfd* fd)
{
	for (std::vector<Http_connection*>::iterator it = connection_fd.begin(); it != connection_fd.end(); it++)
	{
		if ((*it)->get_pollfd() == fd)
		{
			connection_fd.erase(it);
			break ;
		}
	}
}

void	VirtualServ::launch_serv()
{
	if (port.size() == 0)
		throw(std::exception());
	socket_fd.init_connect(1);
}

void	VirtualServ::set_launched(bool val)
{
	socket_fd.set_launched(val);
}

void	VirtualServ::set_name(const std::string &name)
{
	this->server_name = name;
}

std::string	VirtualServ::get_name() const
{
	return (server_name);
}

void	VirtualServ::handle_connection(int &event, Webserv& serv)
{
	for (unsigned long i = 0; i < connection_fd.size() && event > 0; i++)
	{
		std::cout << connection_fd[i]->get_pollfd()->revents << std::endl;
		if ((connection_fd[i]->get_pollfd()->revents & POLLIN) != 0)
		{
			try
			{
				connection_fd[i]->set_request(Request::parsedRequest(connection_fd[i]->get_pollfd()->fd));
				connection_fd[i]->get_pollfd()->events = POLLOUT;
				event--;
			}
			catch(std::exception &a)
			{
				std::cout<<"removed" << std::endl;
				Http_connection* temp = connection_fd[i];
				serv.erase(connection_fd[i]->get_pollfd()->fd);
				delete temp;
				connection_fd.erase(connection_fd.begin() + i);
				// remove_connection(connection_fd[i]->get_pollfd());
				event--;
			}
		}
		else if ((connection_fd[i]->get_pollfd()->revents & POLLOUT) != 0 && connection_fd[i]->get_request())
		{
			connection_fd[i]->get_request()->response(connection_fd[i]->get_pollfd()->fd);
			connection_fd[i]->get_pollfd()->events = POLLIN;
			event--;
		}
	}
}

void	VirtualServ::set_port(const std::string &port)
{
	Addrinfo	test(AF_INET, SOCK_STREAM, 0, AI_PASSIVE, port);
	Fsocket		temp(test);

	socket_fd = temp;
	this->port = port;
}

void	VirtualServ::set_fd(int temp)
{
	socket_fd.set_fd(temp);
}

std::string	VirtualServ::get_port() const
{
	return (port);
}

int	VirtualServ::get_fd() const
{
	return (socket_fd.get_fd());
}