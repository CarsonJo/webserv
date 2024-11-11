#ifndef REQUEST_HPP
# define REQUEST_HPP
#include <string>
#include <unistd.h>
#include <exception>
#include <iostream>
#include "../Webserv/VirtualServ.hpp"

class VirtualServ;

class Request
{
	public :

		static Request* parsedRequest(int fd);
		Request();
		virtual ~Request();
		virtual void	response(int fd, VirtualServ* serv) = 0;

	protected:

		static Request* checkRequest(const std::string& temp);
		std::string	target;
		std::string	protocole_version;
		int		content_length;

};

class Get : public Request
{
	public :
		Get();
		Get(const std::string& target);
		~Get();
		void	response(int fd, VirtualServ* serv);

	private :

};

class Post : public Request
{
	public :
		Post();
		Post(const std::string& target);
		~Post();
		void	response(int fd, VirtualServ* serv);

	private :

};

class Delete : public Request
{
	public :
		Delete();
		Delete(const std::string& target);
		~Delete();
		void	response(int fd, VirtualServ* serv);

	private :

};
#endif