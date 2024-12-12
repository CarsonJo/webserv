#ifndef POST_HPP
#define POST_HPP
#include <string>
#include <sys/types.h>
#include <sys/wait.h>
#include "Request.hpp"

class Request;

class Post : public Request
{
	public :
		Post();
		Post(const std::string& target);
		~Post();
		int	response(int fd);
		virtual std::string	type();
	private :

};

#endif