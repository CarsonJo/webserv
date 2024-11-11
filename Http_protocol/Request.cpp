#include "Request.hpp"

Request::Request() : target(""), protocole_version(""), content_length(0)
{

}

Request::~Request()
{

}

Request* Request::checkRequest(const std::string& temp)
{
	if (temp == "GET")
		return (new Get());
	else if (temp == "POST")
		return (new Post());
	else if (temp == "DELETE")
		return (new Delete());
	throw(std::exception());
}

std::string	next_word(std::string& sub, std::size_t& i)
{
	std::size_t	pos = sub.substr(i).find(' ');
	std::string	ret;

	if (pos == std::string::npos)
		throw(std::exception());
	ret = sub.substr(i, pos);
	i += pos;
	return (ret);
}

Request* Request::parsedRequest(int fd)
{
	Request*		ret;
	char			line[500] = {0};
	std::string		to_parsed;
	std::size_t		pos = 0;

	if (read(fd, &line[0], 500) == -1)
		throw(std::exception());
	to_parsed = std::string(line);
	std::cout << "request : " << to_parsed << std::endl;
	ret = checkRequest(next_word(to_parsed, pos));
	ret->target = next_word(to_parsed, pos);
	std::cout << "target :" << ret->target << std::endl;
	ret->protocole_version = next_word(to_parsed, pos);
	return (ret);
}
/////////////////////////GET///////////////////////////////////////////////////
Get::Get(): Request()
{

}

Get::~Get()
{

}

void	Get::response(int fd, VirtualServ* serv)
{
	std::cout << "got a get" << std::endl;
	std::string	path = serv->get_root();
	path.append(target);
	write(fd, "GET received\n", 14);
}
/////////////////////////POST///////////////////////////////////////////////////

Post::Post() : Request()
{

}

void	Post::response(int fd, VirtualServ* serv)
{
	std::cout << "got a post" << std::endl;
	std::string	path = serv->get_root();
	path.append(target);
	write(fd, "POST received\n", 15);
}

Post::~Post()
{

}
/////////////////////////DELETE///////////////////////////////////////////////////

Delete::Delete() : Request()
{

}
void	Delete::response(int fd, VirtualServ* serv)
{
	std::cout << "got a delete" << std::endl;
	std::string	path = serv->get_root();
	path.append(target);
	write(fd, "DELETE received\n", 17);
}

Delete::~Delete()
{

}