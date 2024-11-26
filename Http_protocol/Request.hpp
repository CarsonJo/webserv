#ifndef REQUEST_HPP
# define REQUEST_HPP
#include <unistd.h>
#include <exception>
#include <iostream>
#include <sstream>
#include <string>
#include "../Webserv/ServerBlock.hpp"
#include "../Webserv/VirtualServ.hpp"
#include "Date.hpp"
#include "Error.hpp"
class VirtualServ;
class ServerBlock;
class Request
{
	public :

		static Request* parsedRequest(int fd, ServerBlock *serv);
		static void	parsed_header(std::string& to_parsed, std::size_t& pos, Request* ret, ServerBlock *serv);
		Request();
		virtual ~Request();
		virtual bool	response(int fd) = 0;

	protected:

		int						handle_error(int fd, const VirtualServ* serv, std::string error_code, int error);
		std::string				error_header(int code);
		static Request* 		checkRequest(const std::string& temp);
		std::string				header;
		std::string				target;
		std::string				protocole_version;
		std::string				content_type;
		std::string				content_length;
		bool					first;
		std::fstream			file_to_send;
		char					buff[8192];
		const VirtualServ		*serv;
};

class Get : public Request
{
	public :
		Get();
		Get(const std::string& target);
		~Get();
		bool	response(int fd);

	private :

};

class Post : public Request
{
	public :
		Post();
		Post(const std::string& target);
		~Post();
		bool	response(int fd);

	private :

};

class Delete : public Request
{
	public :
		Delete();
		Delete(const std::string& target);
		~Delete();
		bool	response(int fd);

	private :

};

#endif