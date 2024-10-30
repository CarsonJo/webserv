#ifndef SERVER_HPP
# define SERVER_HPP
# include <string>
# include <map>
# include <fstream>
# include <exception>
# include <vector>
# include <algorithm>
# include "Addrinfo.hpp"
# include "Fsocket.hpp"
# define GET 1
# define POST 2
# define DELETE 4
class VirtualServ;
typedef void	(*ParseFunction)(const std::string &, VirtualServ&);
class VirtualServ
{
	public :

		void		set_name(const std::string &name);
		void		set_port(const std::string &port);
		void		add_connection(struct pollfd* fd);
		void		remove_connection(struct pollfd* fd);
		int			accept_connect();
		std::string	get_name() const;
		std::string	get_port() const;
		int			get_fd() const;
		void		launch_serv();
		static std::map<std::string, ParseFunction>	server_elem;
		VirtualServ(const Addrinfo& info);

	private :

		VirtualServ();
		VirtualServ(const VirtualServ& to_copy);
		void	operator=(const VirtualServ& to_copy);
		Fsocket							socket_fd;
		std::vector<struct pollfd *>	connection_fd;
		std::string						port;
		std::string						server_name;
		// int			method;
		// std::string	root;
		// std::string	index;
};
#endif