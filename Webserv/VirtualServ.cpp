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
static int myascci(int c)
{
	if (c != '}' && c != ';' && isascii(c))
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
	std::string	temp = ".";

	temp.append(get_value(line, alnum_path));
	if (access(temp.c_str(), X_OK | R_OK | W_OK))
	{
		perror(temp.c_str());
		throw(std::exception());
	}
	serv.set_root(temp);
	std::cout << "root : " << serv.get_root() << std::endl;
}

void	VirtualServ::default_var(const std::string& line, VirtualServ& serv)
{
	std::string	temp = get_value(line, myascci);
	serv.set_default(temp);
}

void	VirtualServ::protocol_var(const std::string& line, VirtualServ& serv)
{
	std::string	temp = get_value(line, myascci);
	if (temp.find("GET") != std::string::npos)
		serv.accepted_protocol |= GET;
	if (temp.find("POST") != std::string::npos)
		serv.accepted_protocol |= POST;
	if (temp.find("DELETE") != std::string::npos)
		serv.accepted_protocol |= DELETE;
	std::cout << "protocole : " << serv.accepted_protocol << std::endl;
}

std::map<std::string, ParseFunction> init_static_elem()
{
	std::map<std::string, ParseFunction> ret;

	ret["server_name"] = VirtualServ::server_var;
	ret["listen"] = VirtualServ::listen_var;
	ret["root"] = VirtualServ::root_var;
	ret["default"] = VirtualServ::default_var;
	ret["protocol"] = VirtualServ::protocol_var;
	return (ret);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

VirtualServ::VirtualServ() : port(""), server_name(""), root(""), accepted_protocol(0)
{

}

VirtualServ::VirtualServ(const VirtualServ& to_copy) : port(to_copy.port)
, server_name(to_copy.server_name), root(to_copy.root), accepted_protocol(to_copy.accepted_protocol)
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

void	VirtualServ::set_default(const std::string& line)
{
	default_page = line;
}


void	VirtualServ::set_name(const std::string &name)
{
	this->server_name = name;
}

void	VirtualServ::set_port(const std::string &name)
{
	this->port = name;
}

void	VirtualServ::set_root(const std::string &root)
{
	this->root = root;
}

std::string	VirtualServ::get_root() const
{
	return (root);
}

std::string	VirtualServ::get_name() const
{
	return (server_name);
}

std::string	VirtualServ::get_port() const
{
	return (port);
}

int VirtualServ::get_protocol() const
{
	return (accepted_protocol);
}

void	VirtualServ::operator=(const VirtualServ& serv)
{
	port = serv.port;
	server_name = serv.server_name;
	root = serv.root;
	default_page = serv.default_page;
	accepted_protocol = serv.accepted_protocol;
}