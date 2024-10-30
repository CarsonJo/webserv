#ifndef CONNECTION_HPP
# define CONNECTION_HPP
# include <poll.h>
# include <cstdlib>
# include <vector>
# include <unistd.h>
# include <sys/socket.h>
# include <fcntl.h>
# include "Addrinfo.hpp"
# include "VirtualServ.hpp"
# define DISCUSS POLLIN | POLLOUT

class Fsocket;
class Webserv
{
	public :

		Webserv();
		~Webserv();
		struct pollfd	*c_arr();
		struct pollfd	*connect_arr();
		int				get_size() const;
		int				get_connect_size() const;
		void			add_serv(const VirtualServ& virtserv);
		void			erase(unsigned long int index);
		void			new_connect();
		void			init_all();
		void			add_master(const VirtualServ& virt_serv);

	private :

		std::vector<struct pollfd>	arr;
		std::vector<int>			original_serv;
		std::vector<VirtualServ>	virtualserv;
		int							master_socket;
		Webserv(const Webserv &to_copy);
		void	add_connect(int fd, int flag, int master_socket);
		void	operator=(const Webserv &to_copy);
};
#endif