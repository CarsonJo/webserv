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
	std::string	ret;

	(void)serv;
	ret.append(" Not Found\r\n").append(Date().get_date());
	return (ret);
}