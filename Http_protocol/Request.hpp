#ifndef REQUEST_HPP
# define REQUEST_HPP
#include <unistd.h>
#include <exception>
#include <iostream>
#include <sstream>
#include <string>
#include "../Webserv/VirtualServ.hpp"

class VirtualServ;

class Request
{
	public :

		static Request* parsedRequest(int fd, VirtualServ *serv);
		Request();
		virtual ~Request();
		virtual bool	response(int fd, VirtualServ* serv) = 0;

	protected:

		static Request* checkRequest(const std::string& temp);
		std::string		header;
		std::string		target;
		std::string		protocole_version;
		std::string		content_type;
		std::string		content_length;
		bool			first;
		std::fstream	file_to_send;
		char			buff[8096];
};

class Get : public Request
{
	public :
		Get();
		Get(const std::string& target);
		~Get();
		bool	response(int fd, VirtualServ* serv);

	private :

};

class Post : public Request
{
	public :
		Post();
		Post(const std::string& target);
		~Post();
		bool	response(int fd, VirtualServ* serv);

	private :

};

class Delete : public Request
{
	public :
		Delete();
		Delete(const std::string& target);
		~Delete();
		bool	response(int fd, VirtualServ* serv);

	private :

};
#endif