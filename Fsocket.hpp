#ifndef FSocket_HPP
# define FSocket_HPP
# include <unistd.h>
# include <sys/socket.h>
# include <exception>
# include <string>
# include <fcntl.h>
# include "Addrinfo.hpp"
# define LISTEN 15
class Fsocket
{
	private:
		int			fd;
		Addrinfo	my_info;
		Fsocket();
	public:
		int		get_fd() const;
		int		accept_connect();
		template <typename A>
		void	init_connect(const A& opt){
			setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(A));
			if (bind(fd, my_info.get_addr(), my_info.get_addrlen()) < 0)
				throw(std::exception());
			if (listen(fd, LISTEN))
				throw(std::exception());
		};
		Fsocket(const Fsocket& to_copy);
		Fsocket(const Addrinfo &info);
		void	operator=(const Fsocket& to_copy);
		~Fsocket();
};

#endif