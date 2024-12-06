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
	throw(std::exception());
}

int	Get::response(int fd)
{
	if (err)
		return (err.trap_card_activate());
	std::string	path = serv->get_root();

	if (!first)
	{
		if (!(serv->get_protocol() & GET))
			return (Error::handle_error(fd, serv, "405", 405));
		Date a;
		if (target == "/")
			target = "/index.html";
		path.append(target);
		std::cout << path << std::endl;
		if (access(path.c_str(),F_OK | R_OK))
			return (Error::handle_error(fd, serv, "404", 404));
		file_to_send.open(path.c_str(), std::fstream::in | std::ios::binary);
		if (!file_to_send.is_open())
			throw(std::exception());
		content_length = find_file_size(file_to_send);
		content_length.append("\r\n");
		try
		{
			content_type = find_content_type(target);
		}
		catch(const std::exception& e)
		{
			return (Error::handle_error(fd, serv, "405", 405));
		}
		std::cout << "content_length: " << content_length << std::endl;
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

std::string	Get::type()
{
	return ("GET");
}