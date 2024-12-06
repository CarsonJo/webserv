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
// # define REP_SERVER "/home/cjozefzo/Documents/cursus42/webserv"
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
		~VirtualServ();
		void		set_name(const std::string& name);
		void		set_root(const std::string& root);
		void		set_default(const std::string& page);
		void		set_port(const std::string& port);
		std::string	get_name() const;
		std::string	get_port() const;
		std::string	get_root() const;
		int			get_protocol() const;
		static std::map<std::string, ParseFunction>	server_elem;
		void	operator=(const VirtualServ& to_copy);

	private :


		static void	root_var(const std::string& line, VirtualServ &serv);
		static void	listen_var(const std::string &line, VirtualServ &serv);
		static void	server_var(const std::string &line, VirtualServ &serv);
		static void	default_var(const std::string &line, VirtualServ &serv);
		static void	protocol_var(const std::string &line, VirtualServ &serv);

		std::string	port;
		std::string	server_name;
		std::string	root;
		std::string	default_page;
		int			accepted_protocol;
		// int			method;

		// std::string	index;
};
#endif