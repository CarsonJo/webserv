#include <poll.h>
#include <string>
#include <iostream>
#include <errno.h>
#include "Connection.hpp"
#include "Addrinfo.hpp"
#include "FSocket.hpp"

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
			char buff[25];
			poll_test.new_connect(info);
			std::cout << "error poll :" << ((poll_test.c_arr())[0].revents & POLLIN) << " " <<
			((poll_test.c_arr())[1].revents & POLLIN) << std::endl;
			err = read((poll_test.c_arr())[1].fd, &buff, sizeof(buff));
			if (err == -1)
			{
				std::cout << errno << std::endl;
				return (0);
			}
			std::cout << "new connection" << std::endl;
		}
	}
}