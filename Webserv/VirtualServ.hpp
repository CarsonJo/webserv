#ifndef SERVER_HPP
# define SERVER_HPP
# include <string>
# include <map>
# include <fstream>
# include <exception>
# include <vector>
# include <algorithm>
# include <poll.h>
# include "Addrinfo.hpp"
# include "Fsocket.hpp"
# include "Webserv.hpp"
# include "Http_connection.hpp"
# include "../Http_protocol/Request.hpp"

class VirtualServ;
class Webserv;
typedef void	(*ParseFunction)(const std::string &, VirtualServ&);

class VirtualServ
{
	public :

		VirtualServ();
		VirtualServ(const VirtualServ& to_copy);
		VirtualServ(const Addrinfo& info);
		~VirtualServ();
		void		set_fd(int temp);
		void		set_name(const std::string &name);
		void		set_port(const std::string &port);
		void		set_launched(bool val);
		void		add_connection(struct pollfd* fd);
		void		handle_connection(int &event, Webserv& serv);
		void		remove_connection(struct pollfd* fd);
		int			accept_connect();
		std::string	get_name() const;
		std::string	get_port() const;
		int			get_fd() const;
		void		launch_serv();
		static std::map<std::string, ParseFunction>	server_elem;

	private :

		void	operator=(VirtualServ& to_copy);
		Fsocket							socket_fd;
		std::vector<Http_connection*>	connection_fd;
		std::string						port;
		std::string						server_name;
		// int			method;
		// std::string	root;
		// std::string	index;
};
#endif