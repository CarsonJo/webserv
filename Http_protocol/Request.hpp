#ifndef REQUEST_HPP
# define REQUEST_HPP
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <dirent.h>
#include <exception>
#include <iostream>
#include <sstream>
#include <string>
#include "../Webserv/ServerBlock.hpp"
#include "../Webserv/VirtualServ.hpp"
#include "Date.hpp"
#include "Get.hpp"
#include "Post.hpp"
#include "Delete.hpp"
#include "Error.hpp"
#include "Http_header.hpp"
# define CLOSE 2
# define PROTOCOLE "Http/1.1"
# define GATEWAY "CGI/1.1"
class VirtualServ;
class ServerBlock;

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

		static Request*		parsedRequest(int fd, ServerBlock *serv);
		static int			parsed_header(std::string& to_parsed, std::size_t& pos, Request* ret, ServerBlock *serv, int fd);
		static std::string	parse_response(char *buff, Request* req);
		static void	parsed_body(std::string& to_parsed, std::size_t& pos, Request* ret, ServerBlock *serv, int fd);
		Request();
		virtual ~Request();
		virtual int			response(int fd) = 0;
		virtual std::string	type() = 0;
		static std::map<std::string, Parsed_header>	header_func;

		void								set_location(const std::string& str);
		void								set_status(const std::string& str);
		void								set_protocole_version(const std::string& str);
		void								set_content_length(const std::string& str);
		void								set_content_type(const std::string& str);
		void								set_serv(const VirtualServ* to_set);
		std::map<std::string, std::string>&	get_cgi_env();

	protected:

		int						fd;
		std::string				error_header(int code);
		static Request* 		checkRequest(const std::string& temp);
		std::map<std::string, std::string>			cgi_env;
		std::string				location;
		std::string				status;
		std::string				protocole_version;
		std::string				content_length;
		std::string				content_type;
		std::string				header;
		std::string				target;
		std::string				body;
		bool					is_cgi;
		bool					is_upload;
		bool					first;
		std::fstream			file_to_send;
		char					buff[8192];
		Error					err;
		const VirtualServ		*serv;
};



#endif