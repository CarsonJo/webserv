#ifndef FSocket_HPP
# define FSocket_HPP
# include <unistd.h>
# include <sys/socket.h>
# include <exception>
# include <fcntl.h>
# include "Addrinfo.hpp"
# define LISTEN 15
class FSocket
{
	private:
		FSocket();
		FSocket(const FSocket& to_copy);
		void	operator=(const FSocket& to_copy);
		int	fd;

	public:
		int		get_fd();
		template <typename A>
		void	init_connect(Addrinfo &info, const A& opt){
			setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(A));
			if (bind(fd, info.get_addr(), info.get_addrlen()) < 0)
				throw(std::exception());
			if (listen(fd, LISTEN))
				throw(std::exception());
		};
		FSocket(const Addrinfo &info);
		FSocket(int fd);
		~FSocket();
};

#endif