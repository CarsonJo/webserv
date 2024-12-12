#ifndef GET_HPP
#define GET_HPP
#include <string>
#include <sys/wait.h>
#include "Request.hpp"
#define CGI 1
#define AUTOINDEX 2
#define GETFILE 3

class Request;

class Get : public Request
{
	public :

		Get();
		Get(const std::string& target);
		~Get();
		int	response(int fd);
		int set_up_cgi(int fd);
		int	cgi_handler(int fd);
		std::string	type();

	private :
		//mettre cgi dans une classe a part
		int	send_file();
		int	send_header();
		void	cgi_header();
		void	set_var_env();
		//autoindex;
		int		method;

		int		p_write[2];
		int		p_read[2];
		bool	children;
		pid_t	pid;
		std::fstream	file_to_send;
};

#endif