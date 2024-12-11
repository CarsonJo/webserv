#include "Request.hpp"

std::map<std::string, Parsed_header> make_header_func();


const char *Test::what() const throw()
{
	return ("test");
}
//verifier tous les fields
Request::Request() : protocole_version(""), content_length(""),  content_type(""), target(""), first(0)
, serv(0)
{
	for (int i = 0; i < 8192; i++)
		buff[i] = 0;
}

Request::~Request()
{

}

void	Request::set_auth(const std::string& str)
{
	auth = str;
}

void	Request::set_content_length(const std::string& str)
{
	content_length = str;
}

void	Request::set_content_type(const std::string& str)
{
	content_type = str;
}

void	Request::set_protocole_version(const std::string& str)
{
	protocole_version = str;
}

void	Request::set_status(const std::string& str)
{
	status = str;
}

void	Request::set_location(const std::string& str)
{
	location = str;
}

void	Request::set_serv(const VirtualServ* to_set)
{
	serv = to_set;
}

void	Request::set_target(const std::string& str)
{
	target = str;
}

void	Request::set_body(const std::string& str)
{
	body = str;
}

void	Request::set_upload(const std::string& str)
{
	upload = str;
}

void	Request::set_error(int fd, const VirtualServ *s, const std::string& str, int code)
{
	err.set_error(fd, s, str, code);
}

void	Request::add_env(const std::string& str, const std::string& value)
{
	cgi_env[str] = value;
}

void	Request::set_fd(int val)
{
	fd = val;
}

void	Request::set_route(const Route& str)
{
	route =str;
}

std::string	Request::get_location() const
{
	return (location);
}

std::string	Request::get_status() const
{
	return (status);
}

std::string	Request::get_content_length() const
{
	return (content_length);
}

const std::string&	Request::get_body() const
{
	return (body);
}

std::string	Request::get_content_type() const
{
	return (content_type);
}

std::string	Request::get_protocole_version() const
{
	return (protocole_version);
}

std::string	Request::get_target() const
{
	return (target);
}

std::string	Request::get_upload() const
{
	return (upload);
}

std::string	Request::get_auth() const
{
	return (auth);
}

const Route&	Request::get_route() const
{
	return (route);
}

int	Request::get_fd() const
{
	return (fd);
}

const VirtualServ*	Request::get_serv() const
{
	return (serv);
}

std::map<std::string, std::string>& Request::get_cgi_env()
{
	return (cgi_env);
}