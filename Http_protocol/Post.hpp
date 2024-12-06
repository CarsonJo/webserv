#ifndef POST_HPP
#define POST_HPP
#include <string>
#include "Request.hpp"
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