#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <arpa/inet.h>
#include <unistd.h>
#include <string>

void discuss(int fd)
{
	char temp[50];
	write(fd, "hello_world\n", 12);
	while (read(fd, &temp, 50) > 0)
	{
		std::cout << temp;
		std::string response;
		std::getline(std::cin, response);
		response.append("\n");
		write(fd, response.c_str(), response.length());
	}
}

int main()
{
	struct	addrinfo		hints, *res;
	char	ipstr[INET_ADDRSTRLEN];
	int		status;

	std::memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_IP;
	hints.ai_flags = AI_PASSIVE;
	hints.ai_canonname = 0;
	hints.ai_addr = 0;
	hints.ai_next = 0;
	if ((status = getaddrinfo(0, "3246", &hints, &res)) != 0)
	{
		std::cerr << "error addrinfo" << std::endl;
		return (0);
	}
	for (struct addrinfo *it = res; it != 0; it = it->ai_next)
	{
		struct	sockaddr_in		temp;

		memcpy(&temp, &it->ai_addr, sizeof(struct sockaddr_in));
		inet_ntop(it->ai_family, &temp.sin_addr, ipstr, sizeof(ipstr));
		std::cout << ipstr << std::endl;
	}
	int fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	int opt = 1;
	setsockopt(fd, SOL_SOCKET, SO_REUSEPORT | SO_REUSEADDR, &opt, sizeof(int));
	if (bind(fd, res->ai_addr, res->ai_addrlen))
	{
		std::cerr << "bind error" << std::endl;
		return (0);
	}
	if (listen(fd, 15))
	{
		std::cerr << "listen error" << std::endl;
		return (0);
	}
	while (1)
	{
		std::cout << "hmm" << std::endl;
		int fd_temp = accept(fd, res->ai_addr, &res->ai_addrlen);
		discuss(fd_temp);
		write(fd_temp, "hello_world\n", 12);
		std::cout << "after" << std::endl;
	}
}