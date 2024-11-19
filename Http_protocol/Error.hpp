#ifndef ERROR_HPP
#define ERROR_HPP
#include <string>
#include <map>
#include "../Webserv/VirtualServ.hpp"

class VirtualServ;
typedef	std::string (*Error_function)(VirtualServ* serv);

class Error
{
	public :
		static std::string	get_error(int code, VirtualServ* serv);
	private :
		static std::map<int, Error_function> function_arr;
		Error();
};
#endif