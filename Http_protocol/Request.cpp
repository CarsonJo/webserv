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

}

Request::~Request()
{

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

std::map<std::string, std::string>& Request::get_cgi_env()
{
	return (cgi_env);
}