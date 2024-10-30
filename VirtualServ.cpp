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
	connection_fd.push_back(fd);
}

void	VirtualServ::remove_connection(struct pollfd* fd)
{
	std::vector<struct pollfd*>::iterator it = std::find(connection_fd.begin(), connection_fd.end(), fd);

	if (it != connection_fd.end())
		connection_fd.erase(it);
}

void	VirtualServ::launch_serv()
{
	socket_fd.init_connect(1);
}

void	VirtualServ::set_name(const std::string &name)
{
	this->server_name = name;
}

std::string	VirtualServ::get_name() const
{
	return (server_name);
}

void	VirtualServ::set_port(const std::string &port)
{
	Addrinfo test(AF_INET, SOCK_STREAM, 0, AI_PASSIVE, port);
	socket_fd = Fsocket(test);
}

std::string	VirtualServ::get_port() const
{
	return (port);
}

int	VirtualServ::get_fd() const
{
	return (socket_fd.get_fd());
}