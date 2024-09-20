#ifndef CONNECTION_HPP
# define CONNECTION_HPP
# include <poll.h>
# include <cstdlib>
# include <vector>
# include <unistd.h>
# include <sys/socket.h>
# include <fcntl.h>
# include "Addrinfo.hpp"
# define BASE_ALLOC 1000
# define DISCUSS POLLIN | POLLOUT
# define ALLOC_ERROR 1

class FSocket;
class Connection
{
	public :

		Connection();
		~Connection();
		struct pollfd	*c_arr();
		int				get_size() const;
		void			add(int fd, int flag);
		void			erase(unsigned long int index);
		void			new_connect(Addrinfo& info);

	private :

		std::vector<struct pollfd>	arr;
		unsigned long int			nfds;
		Connection(const Connection &to_copy);
		void	operator=(const Connection &to_copy);
};
#endif