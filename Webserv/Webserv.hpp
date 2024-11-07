#ifndef CONNECTION_HPP
# define CONNECTION_HPP
# include <poll.h>
# include <cstdlib>
# include <vector>
# include <map>
# include <unistd.h>
# include <sys/socket.h>
# include <fcntl.h>
# include "Addrinfo.hpp"
# include "VirtualServ.hpp"
# define DISCUSS POLLIN | POLLOUT

class Fsocket;
class VirtualServ;
class Request;
class Webserv
{
	public :

		Webserv();
		~Webserv();
		struct pollfd	*c_arr();
		struct pollfd	*connect_arr();
		int				get_size() const;
		int				get_connect_size() const;
		void			handle_recv(int &event);
		void			add_serv(VirtualServ* virtserv);
		void			erase(int fd);
		void			new_connect(int& event);
		void			init_all();
		void			add_master(const VirtualServ& virt_serv);

	private :

		std::vector<struct pollfd>	arr;
		std::map<int, VirtualServ*>	linkServ;
		std::map<int, Request*>		request;
		std::vector<VirtualServ *>	virtualserv;
		int							master_socket;
		Webserv(const Webserv &to_copy);
		void	add_connect(int fd, int flag, VirtualServ *link);
		void	operator=(const Webserv &to_copy);
};
#endif