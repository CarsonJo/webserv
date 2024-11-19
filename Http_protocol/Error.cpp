#include "Error.hpp"

std::string	e405(VirtualServ* serv);
std::string	e404(VirtualServ* serv);
std::string	e403(VirtualServ* serv);

std::map<int, Error_function> fill_function()
{
	std::map<int, Error_function> ret;

	ret[405] = e405;
	ret[403] = e403;
	ret[404] = e404;
	return (ret);
}

std::map<int, Error_function>Error::function_arr = fill_function();

std::string	Error::get_error(int code, VirtualServ* serv)
{
	return (function_arr.at(code)(serv));
}

std::string	e405(VirtualServ* serv)
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

std::string	e403(VirtualServ* serv)
{
	std::string	ret;

	(void)serv;
	ret.append(" Forbidden\r\n").append(Date().get_date());
	return (ret);
}

std::string	e404(VirtualServ* serv)
{
	std::string		ret;
	// std::ifstream	f;
	// char			buff[S404];
	// int a;
	(void)serv;
	// f.open(ROOT, std::fstream::in);
	// if (!f.is_open())
	// 	std::cout << "asdfasdf" << std::endl;
	// f.read(&buff[0], S404);
	// a = f.gcount();
	// std::cout << a << buff << std::endl;
	ret.append(" Not Found\r\n");//.append("Content-Length: ").append(SIZE_404).append("\r\nContent-Type: text/html\r\n\r\n")
	//.append(std::string(buff));

	return (ret);
}