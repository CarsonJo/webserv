#include "Get.hpp"

Get::Get(): Request()
{

}

Get::~Get()
{

}

static std::string find_file_size(std::fstream& file)
{
	std::streampos temp = file.tellg();

	file.seekg(0, file.beg);
	std::ifstream::pos_type begin = file.tellg();
	file.seekg(0, file.end);
	std::stringstream	stream;
	stream << (static_cast<long>(file.tellg() - begin));
	file.seekg(temp);
	return (stream.str());
}

//utiliser map pour opti la rechherche
static std::string find_content_type(std::string& file)
{
	if (file.find(".jpg") != std::string::npos)
		return("image/jpeg\r\n");
	else if (file.find(".html") != std::string::npos)
		return("text/html\r\n");
	else if (file.find(".css") != std::string::npos)
		return("text/css\r\n");
	else if (file.find(".js") != std::string::npos)
		return("application/javascript\r\n");
	else if (file.find(".png") != std::string::npos)
		return("image/png\r\n");
	else if (file.find(".eot") != std::string::npos)
		return("application/vnd.ms-fontobject\r\n");
	else if (file.find(".svg") != std::string::npos)
		return("image/svg+xml\r\n");
	else if (file.find(".woff") != std::string::npos)
		return("font/woff\r\n");
	else if (file.find(".woff2") != std::string::npos)
		return("font/woff2\r\n");
	return ("");
}

static void make_header(std::string& header, std::string& content_length, std::string& content_type)
{
	header = "HTTP/1.1 200 OK\r\n";
	header.append("Content-Length: ").append(content_length);
	if (content_type.size() != 0)
		header.append("Content-Type: ").append(content_type);
	header.append("\r\n");
}

int	Get::send_file()
{
	file_to_send.read(&buff[0], 8192);
	if (file_to_send.rdstate() & std::fstream::badbit)
	{
		first = 0;
		file_to_send.close();
		return (1);
	}
	write(fd, buff, file_to_send.gcount());
	if (file_to_send.gcount() < 8192)
	{
		first = 0;
		file_to_send.close();
		return (1);
	}
	return (0);
}

int	Get::send_header()
{
	std::string	header;
	std::string path = target;

	if (access(path.c_str(), F_OK | R_OK))
		return (Error::handle_error(fd, serv, "403", 403));//a check
	file_to_send.open(path.c_str(), std::fstream::in | std::ios::binary);
	if (!file_to_send.is_open())
		return (Error::handle_error(fd, serv, "403", 403));//a check
	content_length = find_file_size(file_to_send).append("\r\n");
	content_type = find_content_type(target);
	make_header(header, content_length, content_type);
	write(fd, header.c_str(), header.size());
	return (0);
}

int	Get::response(int fd)
{
	if (!first)
	{
		first = 1;
		std::cerr << "heho: " <<  target << std::endl;
		if (err)
			return (err.trap_card_activate());
		if (!(route.get_methods() & GET))
			return (Error::handle_error(fd, serv, "405", 405));//a changer en method not allowed
		if (target.find(".php") != std::string::npos && route.is_cgi_enabled())
			method = CGI;
		else if (is_directory(target))
		{
			if (route.get_default().size() > 0)
			{
				method = GETFILE;
				target = route.get_default();
				std::cerr << target << std::endl;
				return (send_header());
			}
			else if (route.is_autoindex())
				method = AUTOINDEX;
			else
				return (Error::handle_error(fd, serv, "403", 403));
		}
		else
		{
			method = GETFILE;
			return (send_header());
		}
	}
	else
	{
		switch (method){
			case GETFILE:
				return (send_file());
			case AUTOINDEX:
				//inserer autoindex
				break;
			case CGI:
				//inserer cgi_handler;
				break;
		}
	}
	return (0);
}

std::string	Get::type()
{
	return ("GET");
}