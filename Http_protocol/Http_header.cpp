#include "Http_header.hpp"

static std::string	next_word(std::string& sub, std::size_t& i, const char* delim)
{
	std::string	ret = sub.substr(i);
	std::size_t start = ret.find_first_not_of(delim);
	if (start == std::string::npos)
		throw std::exception();
	ret = ret.substr(start);
	std::size_t	pos = ret.find_first_of(delim);

	if (pos == std::string::npos)
	{
		ret = ret.substr(0);
		throw std::exception();
	}
	else
	{
		ret = ret.substr(0, pos);
		i += pos + start;
	}
	return (ret);
}

void	Http_header::location(Request *req, std::string& toRead, std::size_t& pos, ServerBlock *serv)
{
	std::string	temp = toRead.substr(pos);
	std::size_t	end = temp.find("\n");

	if (end == std::string::npos)
		throw(std::exception());
	req->set_location(next_word(toRead, pos, " \t\r\n"));
}

void	Http_header::content_length(Request *req, std::string& toRead, std::size_t& pos, ServerBlock *serv)
{
	std::string	temp = toRead.substr(pos);
	std::size_t	end = temp.find("\n");

	if (end == std::string::npos)
		throw(std::exception());
	req->set_content_length(next_word(toRead, pos, " \t\r\n"));
}

void	Http_header::content_type(Request *req, std::string& toRead, std::size_t& pos, ServerBlock *serv)
{
	std::string	temp = toRead.substr(pos);
	std::size_t	end = temp.find("\n");

	if (end == std::string::npos)
		throw(std::exception());
	req->set_content_type(next_word(toRead, pos, " \t\r\n"));
}

void	Http_header::status(Request *req, std::string& toRead, std::size_t& pos, ServerBlock *serv)
{
	std::string	temp = toRead.substr(pos);
	std::size_t	end = temp.find("\n");

	if (end == std::string::npos)
		throw(std::exception());
	req->set_status(next_word(toRead, pos, " \t\r\n"));
}

void	Http_header::host(Request *req, std::string& toRead, std::size_t& pos, ServerBlock *serv)
{
	std::string	temp = toRead.substr(pos);
	std::size_t	end = temp.find("\n");

	if (end == std::string::npos)
		throw(std::exception());
	req->set_serv(serv->find(next_word(toRead, pos, " \t\r\n:"), next_word(toRead, pos, " \t\r\n")));
}

void	Http_header::location(Request *req, std::string& toRead, std::size_t& pos)
{

}