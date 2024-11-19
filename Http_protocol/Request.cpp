#include "Request.hpp"

Request::Request() : target(""), protocole_version(""), content_type(""), content_length(""), first(0)
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
	std::string	ret = sub.substr(i);
	std::size_t start = ret.find_first_not_of(" \t");
	ret = ret.substr(start);
	std::size_t	pos = ret.find(' ');

	if (pos == std::string::npos)
		throw(std::exception());
	ret = ret.substr(0, pos);
	i += pos;
	return (ret);
}

Request* Request::parsedRequest(int fd, VirtualServ *serv)
{
	Request*		ret;
	char			line[8096] = {0};
	std::string		to_parsed;
	std::size_t		pos = 0;

	if (read(fd, &line[0], 8096) == -1)
		throw(std::exception());
	to_parsed = std::string(line);
	std::cout << "request : " << to_parsed << std::endl;
	ret = checkRequest(next_word(to_parsed, pos));
	ret->target = next_word(to_parsed, pos);
	if (ret->target == "/")
		ret->target = "/index.html";
	std::cout << "target :" << ret->target << std::endl;
	if (ret->target.find(".jpg") != std::string::npos)
		ret->content_type = "image/jpeg\r\n";
	else if (ret->target.find(".js") != std::string::npos)
		ret->content_type = "text/javascript;\r\n";
	else if (ret->target.find(".png") != std::string::npos)
		ret->content_type = "image/png\r\n";
	else if (ret->target.find(".css") != std::string::npos)
		ret->content_type = "text/css\r\n";
	else if (ret->target.find(".ico") != std::string::npos)
		std::cout << "WTFFFFFFFFFFFFFF : " << ret->target << ": WTFVVVVVVVVVVVVVVVV2 : " << serv->get_root().append(ret->target) << std::endl;
	else
		ret->content_type = "text/html\r\n";
	std::cout << "content_type: " << ret->content_type << std::endl;
	std::ifstream f;
	f.open(serv->get_root().append(ret->target).c_str(), std::ios_base::in);
	if (!f.is_open())
		return (ret);
	f.seekg(0, f.beg);
	std::ifstream::pos_type begin = f.tellg();
	f.seekg(0, f.end);
	std::stringstream	stream;
	stream << (static_cast<long>(f.tellg() - begin));
	ret->content_length = stream.str();
	ret->content_length.append("\r\n");
	std::cout << "content_length: " << ret->content_length << std::endl;
	ret->protocole_version = next_word(to_parsed, pos);
	std::cout << "protocole:" << ret->protocole_version << std::endl;
	f.close();
	return (ret);
}

int	Request::handle_error(int fd, VirtualServ* server, std::string error_code, int error)
{
	std::string header;

	header.append("HTTP/1.1 ").append(error_code).append(Error::get_error(error, server));
	std::cout << "ERROR: " << header << std::endl;
	write(fd, header.c_str(), header.size());
	return (1);
}

/////////////////////////GET///////////////////////////////////////////////////
Get::Get(): Request()
{

}

Get::~Get()
{

}

bool	Get::response(int fd, VirtualServ* serv)
{
	std::string	path = serv->get_root();

	if (!first)
	{
		if (!(serv->get_protocol() & GET))
			return (handle_error(fd, serv, "405", 405));
		Date a;
		path.append(target);
		std::cout << path << std::endl;
		if (access(path.c_str(),F_OK | R_OK))
			return (handle_error(fd, serv, "404", 404));
		file_to_send.open(path.c_str(), std::fstream::in);
		if (!file_to_send.is_open())
			throw(std::exception());
		header = "HTTP/1.1 200 OK\r\n";
		header.append("Content-Type: ").append(content_type).append("Content-Length: ").append(content_length)
		.append(a.get_date()).append("\r\n");
		write(fd, header.c_str(), header.size());
		first = 1;
		return (0);
	}
	else
	{
		file_to_send.read(&buff[0], 8192);
		int a = file_to_send.gcount();
		int b = write(fd, buff, file_to_send.gcount());
		(void) a;
		(void) b;
		if (file_to_send.gcount() < 8192)
		{
			first = 0;
			file_to_send.close();
			return (1);
		}
		return (0);
	}
	return (0);
}
/////////////////////////POST///////////////////////////////////////////////////

Post::Post() : Request()
{

}

bool	Post::response(int fd, VirtualServ* serv)
{
	std::cout << "got a post" << std::endl;
	if (!(serv->get_protocol() & POST))
		return (handle_error(fd, serv, "405", 405));
	std::string	path = serv->get_root();
	path.append(target);
	write(fd, "POST received\n", 15);
	return (0);
}

Post::~Post()
{

}
/////////////////////////DELETE///////////////////////////////////////////////////

Delete::Delete() : Request()
{

}

bool	Delete::response(int fd, VirtualServ* serv)
{
	std::cout << "got a delete" << std::endl;
	if (!(serv->get_protocol() & DELETE))
		return (handle_error(fd, serv, "405", 405));
	std::string	path = serv->get_root();
	path.append(target);
	if (access(path.c_str(), F_OK))
		return (handle_error(fd, serv, "404", 404));
	if (unlink(path.c_str()))
		return (handle_error(fd, serv, "403", 403));

	std::string	response;
	response.append("HTTP/1.1 204 No Content\r\n").append(Date().get_date()).append("\r\n");
	write(fd, response.c_str(), response.size());
	return (1);
}

Delete::~Delete()
{

}