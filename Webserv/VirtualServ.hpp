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
# define GET 1
# define POST 2
# define DELETE 4

class VirtualServ;
class Webserv;
typedef void	(*ParseFunction)(const std::string &, VirtualServ&);

class VirtualServ
{
	public :

		friend std::map<std::string, ParseFunction> init_static_elem();
		VirtualServ();
		VirtualServ(const VirtualServ& to_copy);
		VirtualServ(const Addrinfo& info);
		~VirtualServ();
		void		set_fd(int temp);
		void		set_name(const std::string& name);
		void		set_port(const std::string& port);
		void		set_root(const std::string& root);
		void		set_launched(bool val);
		void		set_default(const std::string& page);
		int			accept_connect();
		std::string	get_name() const;
		std::string	get_port() const;
		std::string	get_root() const;
		int			get_protocol() const;
		int			get_fd() const;
		void		launch_serv();
		static std::map<std::string, ParseFunction>	server_elem;


	private :


		static void	root_var(const std::string& line, VirtualServ &serv);
		static void	listen_var(const std::string &line, VirtualServ &serv);
		static void	server_var(const std::string &line, VirtualServ &serv);
		static void	default_var(const std::string &line, VirtualServ &serv);
		static void	protocol_var(const std::string &line, VirtualServ &serv);

		void	operator=(VirtualServ& to_copy);
		Fsocket		socket_fd;
		std::string	port;
		std::string	server_name;
		std::string	root;
		std::string	default_page;
		int			accepted_protocol;
		// int			method;

		// std::string	index;
};
#endif