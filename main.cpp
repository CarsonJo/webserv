#include "main.hpp"

int main(int argc, char **argv)
{
	Webserv		poll_test;
	int			err = 0;

	try
	{
		if (argc < 2)
		{
			if (parse_config("config.cf", poll_test))
				return (0);
		}
		else
		{
			if (parse_config(argv[1], poll_test))
				return (0);
		}
	}
	catch(std::exception())
	{
		return (0);
	}
	signal(SIGPIPE, SIG_IGN);
	try
	{
		while (1)
		{
			if ((err = poll(poll_test.c_arr(), poll_test.get_size(), 0)) > 0)
			{
				poll_test.new_connect(err);
				poll_test.handle_recv(err);
			}
		}
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
}