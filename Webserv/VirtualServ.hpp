#ifndef SERVER_HPP
# define SERVER_HPP
# include <string>
# include <map>
# include <fstream>
# include <exception>
# include <vector>
# include <algorithm>
# include <poll.h>
# include <unistd.h>
# include "Addrinfo.hpp"
# include "Fsocket.hpp"
# include "Webserv.hpp"
# include "../Http_protocol/Request.hpp"
# define REP_SERVER "/home/cjozefzo/Documents/cursus42/webserv"

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
		void		set_name(const std::string& name);
		void		set_port(const std::string& port);
		void		set_root(const std::string& root);
		void		set_launched(bool val);
		int			accept_connect();
		std::string	get_name() const;
		std::string	get_port() const;
		int			get_fd() const;
		void		launch_serv();

		static std::map<std::string, ParseFunction>	server_elem;
		static void	root_var(const std::string& line, VirtualServ &serv);
		static void	listen_var(const std::string &line, VirtualServ &serv);
		static void	server_var(const std::string &line, VirtualServ &serv);

	private :

		void	operator=(VirtualServ& to_copy);
		Fsocket		socket_fd;
		std::string	port;
		std::string	server_name;
		std::string	root;
		// int			method;

		// std::string	index;
};
#endif