#include "main.hpp"
#include "Exception.hpp"
#include "VirtualServ.hpp"
#include <map>

static int	search_block(std::fstream &file, std::string &line)
{
	std::size_t	pos = std::string::npos;
	std::string	sub;

	while (pos == std::string::npos && std::getline(file, line))
	{
		pos = line.find("server", 0);
		if (pos == std::string::npos && line.find_first_not_of(" \t") != std::string::npos)
			throw (ConfigException());
		else if (pos != std::string::npos && line.find_first_not_of(" \t") < pos)
			throw (ConfigException());
	}
	if (pos == std::string::npos)
		return (EOF);
	try
	{
		do
		{
			sub = line.substr(pos + 6);
			pos = sub.find_first_not_of(" \t");
			if (pos != std::string::npos)
			{
				if (sub[pos] == '{')
					return (SERVER_BLOCK);
				throw (ConfigException());
			}
			pos = -6;
		} while (std::getline(file, line));
	}
	catch (std::exception &e)
	{
		std::cerr << "exception catched" << std::endl;
	}
	// throw(EmptyFile());
	return (EOF);
}

static void config_server(std::fstream &config, std::string &line, VirtualServ& server)
{
	std::size_t										pos = line.find_first_of('{');
	std::map<std::string, ParseFunction>::iterator	it;
	std::size_t										end;
	std::string										sub;

	if ((pos == std::string::npos && line.find_first_not_of(" \t{") != std::string::npos))
		throw(std::exception());
	do{
		pos = line.substr(pos + 1).find_first_not_of(" \t");
		if (pos == std::string::npos)
		{
			pos = -1;
			continue ;
		}
		if (line[pos] == '}')
			break ;
		sub = line.substr(pos);
		end = sub.find_first_of(" \t;}");
		if (end == std::string::npos || sub[end] == '}')
			throw(std::exception());
		it = VirtualServ::server_elem.find(sub.substr(0, end));
		if (it == VirtualServ::server_elem.end())
			throw(std::exception());
		it->second(sub.substr(end), server);
		if (sub.substr(sub.find(';') + 1).find_first_not_of(" \t") != std::string::npos)
			throw(std::exception());
		pos = -1;
	} while(std::getline(config, line));
}

int	parse_config(std::string name, Webserv& server)
{
	Addrinfo		info(AF_INET, SOCK_STREAM, 0, AI_PASSIVE, "3246");
	VirtualServ		temp_serv(Addrinfo(AF_INET, SOCK_STREAM, 0, AI_PASSIVE, "3246"));
	std::fstream	config;
	std::string		line;

	config.open(name.c_str(), std::ofstream::in);
	if (config.fail())
	{
		std::cout << std::fstream::failbit << std::endl;
		return (0);
	}
	while (search_block(config, line) != EOF)
	{
		config_server(config, line, temp_serv);
		temp_serv.launch_serv();
		server.add_serv(temp_serv);
		server.add_master(temp_serv);
	}
	return (0);
}