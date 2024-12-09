#ifndef REQUEST_HPP
# define REQUEST_HPP
#include <unistd.h>
#include <exception>
#include <iostream>
#include <string>
#include "../Webserv/Route.hpp"
#include "../Webserv/VirtualServ.hpp"
#include "Date.hpp"
#include "Error.hpp"
#include "Http_header.hpp"
#include "../Webserv/lib_function.hpp"
# define CLOSE 2
# define PROTOCOLE "Http/1.1"
# define GATEWAY "CGI/1.1"

class VirtualServ;
class ServerBlock;
class Get;

int	handle_error(int fd, const VirtualServ* server, std::string error_code, int error);

class Test : public std::exception
{
	public :
		const char*	what() const throw();
	private :

};

class Request
{
	public :

		Request();
		virtual ~Request();
		virtual int			response(int fd) = 0;
		virtual std::string	type() = 0;


		std::string							get_location() const;
		std::string							get_status() const;
		std::string							get_content_type() const;
		std::string							get_protocole_version() const;
		std::string							get_content_length() const;
		std::string							get_target() const;
		std::string							get_upload() const;
		const Route&						get_route() const;
		int									get_fd() const;
		const VirtualServ*					get_serv() const;


		void								set_route(const Route& route);
		void								set_fd(int val);
		void								set_location(const std::string& str);
		void								set_status(const std::string& str);
		void								set_protocole_version(const std::string& str);
		void								set_content_length(const std::string& str);
		void								set_content_type(const std::string& str);
		void								set_target(const std::string& str);
		void								set_upload(const std::string& str);
		void								set_serv(const VirtualServ* to_set);
		void								set_error(int fd, const VirtualServ *s, const std::string& str, int code);
		void								add_env(const std::string& str, const std::string& value);
		std::map<std::string, std::string>&	get_cgi_env();
		friend Request* 		checkRequest(const std::string& temp);
		friend int				parsed_header(std::string& to_parsed, std::size_t& pos, Request* ret, ServerBlock *serv, int fd);
		friend std::string		parse_response(char *buff, Request* req);
		friend void				parsed_body(std::string& to_parsed, std::size_t& pos, Request* ret, ServerBlock *serv, int fd);
	protected:

		int						fd;
		std::string				error_header(int code);
		std::map<std::string, std::string>			cgi_env;
		std::string								location;
		std::string								status;
		std::string								protocole_version;
		std::string								content_length;
		std::string								content_type;
		std::string								target;
		std::string								upload;
		bool									first;
		char									buff[8192];
		Error									err;
		const VirtualServ						*serv;
		Route									route;
};



#endif