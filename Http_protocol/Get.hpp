#ifndef GET_HPP
#define GET_HPP
#include <string>
#include "Request.hpp"

class Request;

class Get : public Request
{
	public :
		Get();
		Get(const std::string& target);
		~Get();
		int	response(int fd);
		std::string	type();
	private :

};

#endif