#include "main.hpp"

void	handle_recv(struct pollfd *arr, unsigned long int size)
{
	unsigned long int	i = 0;
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
				write(arr[i].fd, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: 64\r\n\r\n<!DOCTYPE html><html><body><p>messsage received<p></body></html>", 129);
		}
		i++;
	}
	// std::cout << "finished" << std::endl;
}

int main()
{
	Webserv		poll_test;
	int			err = 0;

	parse_config("config.cf", poll_test);
	while (1)
	{
		if ((err = poll(poll_test.c_arr(), poll_test.get_size(), 10)) > 0)
		{
			poll_test.new_connect();
			handle_recv(poll_test.connect_arr(), poll_test.get_connect_size());
		}
	}
}