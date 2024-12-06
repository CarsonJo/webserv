#include "ServerBlock.hpp"

ServerBlock::ServerBlock() :port("")
{

}

ServerBlock::ServerBlock(const Addrinfo& info) : socket_fd(info)
{

}

int	ServerBlock::get_fd() const
{
	return (socket_fd.get_fd());
}

void	ServerBlock::set_fd(int temp)
{
	socket_fd.set_fd(temp);
}

void	ServerBlock::set_port(const std::string &port)
{
	Addrinfo	test(AF_INET, SOCK_STREAM, 0, AI_PASSIVE, port);
	Fsocket		temp(test);

	socket_fd = temp;
	this->port = port;
}

int	ServerBlock::accept_connect()
{
	return(socket_fd.accept_connect());
}

void	ServerBlock::launch_serv()
{
	if (port.size() == 0)
		throw(std::exception());
	socket_fd.init_connect(1);
}

void	ServerBlock::set_launched(bool val)
{
	socket_fd.set_launched(val);
}

void ServerBlock::add(const VirtualServ& serv)
{
	arr[serv.get_name()] = serv; // const assign to non const problem ?

}
// copie partout pas opti
void	ServerBlock::set_default(const VirtualServ& serv)
{
	def = serv;
	add(serv);
}

std::string	ServerBlock::get_port() const
{
	return (port);
}
std::map<std::string, VirtualServ>::iterator	ServerBlock::end()
{
	return (arr.end());
}

VirtualServ* ServerBlock::find(const std::string& name, const std::string& t_port)//retourner un const virtualserv?
{
	std::map<std::string, VirtualServ>::iterator	it = arr.find(name);

	if (it != arr.end())
	{
		if (t_port.size() == 0 && it->second.get_port() == "80")
			return (&it->second);
		else if (t_port == it->second.get_port())
			return (&it->second);
	}
	else if ((name == host || host == DEFAULT_IP) && t_port == port)
		return (&def);
	return (0);
}

VirtualServ* ServerBlock::unique()
{
	if (arr.size() == 1)
		return (&arr.begin()->second);
	return (0);
}

VirtualServ* ServerBlock::get_default()
{
	return (&def);
}