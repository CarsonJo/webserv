#ifndef ERROR_HPP
#define ERROR_HPP
#include <string>
#include <map>
#include "Date.hpp"
#include "../Webserv/VirtualServ.hpp"
#define ROOT "/home/cjozefzo/Documents/cursus42/webserv/root/404.html"
#define SIZE_404	"63"
#define S404		63
class VirtualServ;
typedef	std::string (*Error_function)(const VirtualServ* serv, std::string& header);

class Error
{
	public :

		Error();
		Error(int fd, const VirtualServ* serv, int error, std::string str_error);
		static int					handle_error(int fd, const VirtualServ* serv, std::string error_code, int error);
		static std::string	get_error(int code, const VirtualServ* serv, std::string& header);
		void				set_error(int fd, const VirtualServ* serv, std::string error_code, int error);
		int					trap_card_activate();
		operator	bool() const;
	private :

		int					fd;
		const VirtualServ*	serv;
		int					error;
		std::string			str_error;
		static std::map<int, Error_function> function_arr;
};
#endif