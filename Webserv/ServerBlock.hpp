#ifndef SERVERBLOCK_HPP
# define SERVERBLOCK_HPP
# include <string>
# include <map>
# include "VirtualServ.hpp"
class VirtualServ;

class ServerBlock{

	public :

		ServerBlock();
		ServerBlock(const Addrinfo& info);
		void											add(const VirtualServ& serv);
		std::string										get_port() const;
		std::map<std::string, VirtualServ>::iterator	end();
		VirtualServ*									find(const std::string& name);
		VirtualServ*									unique();
		void											set_fd(int temp);
		void											set_port(const std::string& port);
		void											set_launched(bool val);
		int												accept_connect();
		int												get_fd() const;
		void											launch_serv();

	private :

		Fsocket									socket_fd;
		std::string								port;
		std::map<std::string, VirtualServ>		arr;
};
#endif