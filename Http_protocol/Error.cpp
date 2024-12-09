#include "Error.hpp"

std::string	e406(const VirtualServ* serv);
std::string	e405(const VirtualServ* serv);
std::string	e404(const VirtualServ* serv);
std::string	e403(const VirtualServ* serv);
std::string	e413(const VirtualServ* serv);
std::string	e500(const VirtualServ* serv);
std::string	e400(const VirtualServ* serv);

std::map<int, Error_function> fill_function()
{
	std::map<int, Error_function> ret;

	ret[405] = e405;
	ret[403] = e403;
	ret[404] = e404;
	ret[406] = e406;
	ret[413] = e413;
    ret[500] = e500;
	ret[400] = e400;
	return (ret);
}

Error::Error() : fd(-1), serv(0), error(0), str_error("")
{

}

Error::Error(int fd, const VirtualServ* serv, int error, std::string str_error) :
	fd(fd), serv(serv), error(error), str_error(str_error)
{

}

void	Error::set_error(int fd, const VirtualServ* serv, std::string str_error, int error)
{
	this->fd = fd;
	this->serv = serv;
	this->error = error;
	this->str_error = str_error;
}

int	Error::trap_card_activate()
{
	if (!*this)
		throw(std::exception());
	return (handle_error(fd, serv, str_error, error));
}

std::map<int, Error_function>Error::function_arr = fill_function();

std::string	Error::get_error(int code, const VirtualServ* serv)
{
	return (function_arr.at(code)(serv));
}

int	Error::handle_error(int fd, const VirtualServ* server, std::string error_code, int error)
{
	std::string header;


	header.append("HTTP/1.1 ").append(error_code).append(Error::get_error(error, server));
	std::cerr << "ERROR: " << header << std::endl;
	write(fd, header.c_str(), header.size());
	return (2);
}

Error::operator bool() const
{
	if (fd == -1)
		return (0);
	return (1);
}

std::string	e405(const VirtualServ* serv)
{
	std::string	ret;
	int			prot = serv->get_protocol();

	ret.append(" Method not allowed\r\nAllow: ");
	if (prot & GET)
		ret.append("GET, ");
	if (prot & POST)
		ret.append("POST, ");
	if (prot & DELETE)
		ret.append("DELETE, ");
	ret.append("\r\n");
	return (ret);
}

// Mauvaise synthaxe ou mauvais param
std::string e400(const VirtualServ* serv) {
    std::string ret;
    (void)serv;
    ret.append(" Bad Request\r\n").append(Date().get_date());
    return ret;
}

// Fichier uploade = trop lourd
std::string e413(const VirtualServ* serv) {
    std::string ret;
    (void)serv;
    ret.append(" Payload Too Large\r\n");// placeholder
    return ret;
}

//Prob CGI, config cote client + tt le reste des exceptions non gerees avant
std::string e500(const VirtualServ* serv) {
    std::string ret;
    (void)serv;
    ret.append(" Internal Server Error\r\n").append(Date().get_date());
    return ret;
}

std::string	e406(const VirtualServ* serv)
{
	std::string	ret;

	(void)serv;
	ret.append(" NOT Acceptable\r\n").append(Date().get_date());
	return (ret);
}

std::string	e403(const VirtualServ* serv)
{
	std::string	ret;

	(void)serv;
	ret.append(" Forbidden\r\n").append(Date().get_date());
	return (ret);
}

std::string	e404(const VirtualServ* serv)
{
	std::string		ret;
	// std::ifstream	f;
	// char			buff[S404];
	// int a;
	(void)serv;
	// f.open(ROOT, std::fstream::in);
	// if (!f.is_open())
	// 	std::cerr << "asdfasdf" << std::endl;
	// f.read(&buff[0], S404);
	// a = f.gcount();
	// std::cerr << a << buff << std::endl;
	ret.append(" Not Found\r\n");//.append("Content-Length: ").append(SIZE_404).append("\r\nContent-Type: text/html\r\n\r\n")
	//.append(std::string(buff));

	return (ret);
}