#include "req_function.hpp"

Request* checkRequest(const std::string& temp)
{
	if (temp == "GET")
		return (new Get());
	else if (temp == "POST")
		return (new Post());
	else if (temp == "DELETE")
		return (new Delete());
	throw(std::exception());
}

static std::string	next_word(std::string& sub, std::size_t& i)
{
	std::string	ret = sub.substr(i);
	std::size_t start = ret.find_first_not_of(" \t\r\n");
	if (start == std::string::npos)
		throw std::exception();
	ret = ret.substr(start);
	std::size_t	pos = ret.find_first_of(" \t\r\n");

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

// void	parsed_body(std::string& to_parsed, std::size_t& pos, Request* ret, ServerBlock *serv, int fd)
// {
// 	(void)serv;
// 	(void)fd;
// 	ret->body = to_parsed.substr(pos);
// 	if (std::atoi(ret->content_length.c_str()) < ret->body.size()) //double utilisation atoi utiliser variable
// 		ret->body = ret->body.substr(0, std::atoi(ret->content_length.c_str()));
// }

int	parsed_header(std::string& to_parsed, std::size_t& pos, Request* ret, ServerBlock *serv, int fd)
{
	(void)fd;
	try
	{
		std::map<std::string, Parsed_header>::iterator	it;
		std::string										test;
		std::size_t										temp;
		std::size_t										end = to_parsed.find("\r\n\r\n");

		while (1)
		{
			if (end <= pos + 1)
				return (1);
			test = next_word(to_parsed, pos);
			it = Http_header::header_func.find(test);
			if (it == Http_header::header_func.end())
			{
				temp = to_parsed.substr(pos).find_first_of("\n");
				if (temp == std::string::npos)
					return (0);
				pos += temp;
				continue;
			}
			it->second(ret, to_parsed, pos, serv);
		}
	}
	catch(const std::exception& e)
	{
		return (0);
	}
}

Request* parsedRequest(int fd, ServerBlock *serv)
{
	Request*		ret;
	char			line[8096] = {0};
	std::string		to_parsed;
	std::size_t		pos = 0;
	std::size_t		end = 0;
	ret = 0;
	try
	{
		if (read(fd, &line[0], 8096) == -1)
			throw(std::exception());
		to_parsed = std::string(line);
		end = to_parsed.find("\n");
		std::cout << "request : " << to_parsed << std::endl;
		ret = checkRequest(next_word(to_parsed, pos));
		if (ret->type() == "POST")
			ret->is_cgi = 1;
		ret->fd = fd;
		ret->target = next_word(to_parsed, pos);
		if (ret->target.find("?") != std::string::npos)
		{
			ret->cgi_env["QUERY_STRING="] = ret->target.substr(ret->target.find("?"));// decale de 1 pour ne pas inclure ?
			ret->target = ret->target.substr(0, ret->target.find("?"));
		}
		ret->protocole_version = next_word(to_parsed, pos);
		if (pos > end)
			ret->err.set_error(fd, serv->get_default(), "405", 405);
		std::cout << "protocole:" << ret->protocole_version << std::endl;
		parsed_header(to_parsed, pos, ret, serv, fd);
			// parsed_body(to_parsed, pos, ret, serv, fd);
	}
	catch(const std::exception& e)
	{
		delete ret;
		throw(e);
	}
	return (ret);
}