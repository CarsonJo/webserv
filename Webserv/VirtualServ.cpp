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

static int alnum_path(int c)
{
	if (c == '/' || (c != '}' && isalnum(c)))
		return (1);
	return (0);
}

void	VirtualServ::server_var(const std::string &line, VirtualServ &serv)
{
	//mettre unb name par defaut au cas ou server name apparait deux fois dans le fichier de config
	std::string	temp = get_value(line, myalnum);
	serv.set_name(temp);
}

void	VirtualServ::listen_var(const std::string &line, VirtualServ &serv)
{
	std::string	temp = get_value(line, isdigit);
	serv.set_port(temp);
}

void	VirtualServ::root_var(const std::string& line, VirtualServ &serv)
{
	std::string	temp = REP_SERVER;

	temp.append(get_value(line, alnum_path));
	if (access(temp.c_str(), X_OK | R_OK | W_OK))
	{
		perror(temp.c_str());
		throw(std::exception());
	}
	serv.set_root(temp);
	std::cout << "root : " << temp << std::endl;
}

std::map<std::string, ParseFunction> init_static_elem()
{
	std::map<std::string, ParseFunction> ret;

	ret["server_name"] = VirtualServ::server_var;
	ret["listen"] = VirtualServ::listen_var;
	ret["root"] = VirtualServ::root_var;
	return (ret);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

VirtualServ::VirtualServ() : socket_fd(),  port(""), server_name("")
{

}

VirtualServ::VirtualServ(const VirtualServ& to_copy) : socket_fd(to_copy.socket_fd)
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
VirtualServ::VirtualServ(const Addrinfo& info) : socket_fd(info)
{

}

int	VirtualServ::accept_connect()
{
	return(socket_fd.accept_connect());
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

void	VirtualServ::set_port(const std::string &port)
{
	Addrinfo	test(AF_INET, SOCK_STREAM, 0, AI_PASSIVE, port);
	Fsocket		temp(test);

	socket_fd = temp;
	this->port = port;
}

void	VirtualServ::set_root(const std::string &root)
{
	this->root = root;
}

void	VirtualServ::set_fd(int temp)
{
	socket_fd.set_fd(temp);
}

std::string	VirtualServ::get_name() const
{
	return (server_name);
}

std::string	VirtualServ::get_port() const
{
	return (port);
}

int	VirtualServ::get_fd() const
{
	return (socket_fd.get_fd());
}