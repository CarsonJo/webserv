#include "main.hpp"

int main()
{
	Webserv		poll_test;
	int			err = 0;

	parse_config("config.cf", poll_test);
	try
	{
		while (1)
		{
			if ((err = poll(poll_test.c_arr(), poll_test.get_size(), 100)) > 0)
			{
				std::cout << "enter" << std::endl;
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