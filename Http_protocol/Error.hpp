#ifndef ERROR_HPP
#define ERROR_HPP
#include <string>
#include <map>
#include "../Webserv/VirtualServ.hpp"
#define ROOT "/home/cjozefzo/Documents/cursus42/webserv/root/404.html"
#define SIZE_404	"63"
#define S404		63
class VirtualServ;
typedef	std::string (*Error_function)(const VirtualServ* serv);

class Error
{
	public :
		static std::string	get_error(int code, const VirtualServ* serv);
	private :
		static std::map<int, Error_function> function_arr;
		Error();
};
#endif