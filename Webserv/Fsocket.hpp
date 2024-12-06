#ifndef FSocket_HPP
# define FSocket_HPP
# include <unistd.h>
# include <sys/socket.h>
# include <exception>
# include <string>
# include <fcntl.h>
# include <string.h>
# include <errno.h>
# include <iostream>
# include "Addrinfo.hpp"
# define LISTEN 100
class Fsocket
{
	private:
		int			fd;
		int			launched;
		Addrinfo	my_info;

	public:
		int		get_fd() const;
		void	set_fd(int val);
		void	set_launched(bool val);
		int		accept_connect();
		template <typename A>
		void	init_connect(const A& opt){
			if (launched || fd < 0)
				throw(std::exception());
			setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(A));
			if (bind(fd, my_info.get_addr(), my_info.get_addrlen()) < 0)
			{
				std::cout << strerror(errno) << std::endl;
				throw(std::exception());
			}
			if (listen(fd, LISTEN))
				throw(std::exception());
			launched = 1;
		};
		Fsocket();
		Fsocket(const Fsocket& to_copy);
		Fsocket(const Addrinfo &info);
		void	operator=(const Fsocket& to_copy);
		~Fsocket();
};
#endif