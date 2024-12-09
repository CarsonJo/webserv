#include "Delete.hpp"
Delete::Delete() : Request()
{

}

int	Delete::response(int fd)
{
	std::cerr << "got a delete" << std::endl;
	if (!(serv->get_protocol() & DELETE))
		return (Error::handle_error(fd, serv, "405", 405));
	std::string	path = serv->get_root();
	path.append(target);
	if (access(path.c_str(), F_OK))
		return (Error::handle_error(fd, serv, "404", 404));
	if (unlink(path.c_str()))
		return (Error::handle_error(fd, serv, "403", 403));

	std::string	response;
	response.append("HTTP/1.1 204 No Content\r\n").append(Date().get_date()).append("\r\n");
	write(fd, response.c_str(), response.size());
	return (1);
}

std::string	Delete::type()
{
	return ("DELETE");
}

Delete::~Delete()
{

}