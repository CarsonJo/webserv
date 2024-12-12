#include "Get.hpp"

//directory listing headers

#include <dirent.h>
#include <sys/stat.h>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>

std::string generate_directory_listing(const std::string& directory_path, const std::string& request_path) {
    DIR* dir = opendir(directory_path.c_str());
    if (!dir) {
        throw std::runtime_error("Failed to open directory: " + directory_path);
    }

    std::ostringstream html;
    html << "<html><head><title>Directory Listing</title></head><body>";
    html << "<h1>Index of " << request_path << "</h1>";
    html << "<ul>";

    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL) {
        std::string name = entry->d_name;
        if (name == ".") continue; // Skip current directory
        if (name == "..") {
            html << "<li><a href=\"../\">Parent Directory</a></li>";
            continue;
        }

 			std::string href = request_path;
 	       if (!href.empty() && href[href.size() - 1] != '/') {
            href += '/';
        }
        html << "<li><a href=\"" << href << name << "\">" << name << "</a></li>";
    }

    html << "</ul></body></html>";
    closedir(dir);
    return html.str();
}

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
		if (target.find(".cgi") != std::string::npos && route.is_cgi_enabled())
		{
			method = CGI;
			return (set_up_cgi(fd));
		}
		else if (is_directory(target))
		{
			if (route.get_default().size() > 0)
			{
				method = GETFILE;
				target.append(route.get_default());
				std::cerr << "New target : " << target << std::endl;
				if (access(target.c_str(), F_OK | R_OK) < 0)
					return	(Error::handle_error(fd, serv, "403", 403));
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
				try {

                    std::string html = generate_directory_listing(target, url);


                    std::string header = "HTTP/1.1 200 OK\r\n";
                    std::stringstream ss;
	             	 ss << html.size();
					header.append("Content-Length: ").append(ss.str()).append("\r\n");

                    header.append("Content-Type: text/html\r\n\r\n");


                    if (write(fd, header.c_str(), header.size()) == -1) {
                        return -1;
                    }


                    if (write(fd, html.c_str(), html.size()) == -1) {
                        return -1;
                    }

                    first = 0;
                } catch (const std::exception& e) {
                    return Error::handle_error(fd, serv, "500", 500);
                }
                return 1;
				break;
			case CGI:
				return (cgi_handler(fd));
				break;
		}
	}
	return (0);
}

std::string	Get::type()
{
	return ("GET");
}

static void	set_up_child(int *read, int *write)
{
	if (pipe(read) == -1)
		throw(std::exception());
	if (pipe(write) == -1)
	{
		close(read[0]);
		close(read[1]);
		throw(std::exception());
	}
}

static void stcpy(char *a, const char *b, int size)
{
	int i = 0;

	while (i < size && b[i])
	{
		a[i] = b[i];
		i++;
	}
}

static void	set_my_env(char **envp, std::map<std::string, std::string>& env)
{
	std::map<std::string, std::string>::iterator it = env.begin();
	envp = new char*[20];
	for (int i = 0; i < 20; i++)
		envp[i] = new char[200];
	for (int i = 0; i < 20 && it != env.end(); i++,it++)
		stcpy(envp[i], (it->first + it->second).c_str(), 200);
}

void	Get::set_var_env()
{
		cgi_env["SERVER_PROTOCOLE:"] = PROTOCOLE;
		cgi_env["GATEWAY_INTERFACE:"] = GATEWAY;
		cgi_env["REMOTE_HOST"] = "";
		cgi_env["REMOTE_ADDR"] = "";
		cgi_env["REQUEST_METHOD"] = this->type();
		cgi_env["SERVER_NAME"] = serv->get_name();
		cgi_env["SCRIPT_NAME"] = target;
}

int Get::set_up_cgi(int fd)
{
	std::cout << "TARGET CGI: " << target << std::endl;
	set_up_child(&p_read[0], &p_write[0]);
	set_var_env();
	if (access(target.c_str(), F_OK | X_OK) != 0)
		return (Error::handle_error(fd, serv, "404", 404));
	if ((std::size_t)write(p_write[1], body.c_str(), body.size()) < body.size())//attention write peut bloquer set up les pipe en non bloquant d'abord avec fcntl peut etre?
			throw (std::exception());
	pid = fork();

	if (pid == -1)
		return (Error::handle_error(fd, serv, "405", 405));
	if (!pid)
	{
		char **envp = 0;
		if (dup2(p_write[0], 0) == -1)
			std::exit(0);
		if (dup2(p_read[1], 1) == -1)
			std::exit(0);
		set_my_env(envp, cgi_env);
		if (execve(target.c_str(), envp, envp) == -1)
			std::exit(0);
	}
	else
	{
		children = 1;
		close(p_write[0]);
		if (fcntl(p_read[0], F_SETFL, O_NONBLOCK) < 0)
			throw(std::exception());
		close(p_write[1]);
	}
	return (0);
}

static std::string	next_word(std::string& sub, std::size_t& i) // le mettre dans req_function
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

void	Get::cgi_header()
{
	std::string	header = buff;
	std::size_t	pos = header.find("Status:");
	std::size_t	temp = pos;
	std::string code = "200";

	if (pos != std::string::npos)
	{
		code = next_word(header, pos);
		cgi_response.append("HTTP/1.1 ").append(code).append("\n");
		cgi_response.append(header.substr(0, temp)).append(header.substr(pos));
		std::cout << "Size1 : " << cgi_response.size() << std::endl;
		write(fd, cgi_response.c_str(), cgi_response.size());
	}
	else
	{
		cgi_response.append("HTTP/1.1 200 OK\n");
		cgi_response.append(header);
		std::cout << "Size2 : " << cgi_response.size() << std::endl;
		write(fd, cgi_response.c_str(), cgi_response.size());
	}
}

int	Get::cgi_handler(int fd)
{
	int	status;

	int size = read(p_read[0], &buff[0], 8196);//mettre le read en non bloquant sinon les problemes;
	perror("read fail");
	if (waitpid(pid, &status, WNOHANG) > 0)
		children = 0;
	if (size == -1 || size == 0)
	{
		if (children == 1)
			return (0);
		close(p_read[0]);
		close(p_read[1]);
		return (CLOSE);
	}
	if (cgi_response.size() == 0)
		cgi_header();
	else
		write(fd, buff, size);
	return (0);
}