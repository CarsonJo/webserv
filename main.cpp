#include <poll.h>
#include <string>
#include <iostream>
#include <errno.h>
#include "Connection.hpp"
#include "Addrinfo.hpp"
#include "FSocket.hpp"

void	handle_recv(struct pollfd *arr, unsigned long int size)
{
	unsigned long int	i = 1;
	char				buff[1024];
	int					end;

	while (i < size)
	{
		// std::cout << "in" << std::endl;
		if ((arr[i].revents & POLLIN) != 0)
		{
			if ((end = read(arr[i].fd, &buff, 1024)) < 0)
				std::cout << "ERNNO :" << errno << std::endl;
			write(1, &buff, end);
			if ((arr[i].revents & POLLOUT) != 0)
				write(arr[i].fd, "message receive\n", 16);
		}
		i++;
	}
}

int main()
{
	Addrinfo	info(AF_INET, SOCK_STREAM, 0, AI_PASSIVE, "3246");
	FSocket		main_socket(info);
	Connection			poll_test;
	int					err = 0;

	main_socket.init_connect<int>(info, (int){1});
	poll_test.add(main_socket.get_fd(), POLLIN);
	while (1)
	{
		if ((err = poll(poll_test.c_arr(), poll_test.get_size(), 1000)) > 0)
		{
			poll_test.new_connect(info);
			handle_recv(poll_test.c_arr(), poll_test.get_size());
		}
	}
}