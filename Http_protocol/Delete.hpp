#ifndef DELETE_HPP
#define DELETE_HPP
#include <string>
#include "Request.hpp"
class Delete : public Request
{
	public :
		Delete();
		Delete(const std::string& target);
		~Delete();
		int		response(int fd);
		virtual std::string	type();
	private :

};
#endif