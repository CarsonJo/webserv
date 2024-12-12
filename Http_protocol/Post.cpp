#include "Post.hpp"

Post::Post() : Request()
{

}

int	Post::response(int fd)
{
	// if (err)
	// 	return (err.trap_card_activate());
	// if (!(serv->get_protocol() & POST))
	// 	return (Error::handle_error(fd, serv, "405", 405));
	// if (target.find("cgi") == std::string::npos)
	// 	return (Error::handle_error(fd, serv, "405", 405));
	// std::string	path = serv->get_root();
	// path.append(target);
	// if (access(target.c_str(), X_OK | F_OK))
	// 	return (Error::handle_error(fd, serv, "405", 405));
	// int	p_write[2], p_read[2];
	// set_up_child(&p_read[0], &p_write[0]);
	// pid_t	pid = fork();

	// if (pid == -1)
	// 	return (Error::handle_error(fd, serv, "405", 405));
	// if (!pid)
	// {
	// 	char **envp = 0;
	// 	if (dup2(p_write[0], 0) == -1)
	// 		std::exit(0);
	// 	if (dup2(p_read[1], 1) == -1)
	// 		std::exit(0);
	// 	cgi_env["SERVER_PROTOCOLE:"] = PROTOCOLE;
	// 	cgi_env["GATEWAY_INTERFACE:"] = GATEWAY;
	// 	cgi_env["REMOTE_HOST"] = "";
	// 	cgi_env["REMOTE_ADDR"] = "";
	// 	cgi_env["REQUEST_METHOD"] = this->type();
	// 	cgi_env["SERVER_NAME"] = serv->get_name();
	// 	cgi_env["SCRIPT_NAME"] = target;
	// 	set_my_env(envp, cgi_env);
	// 	if (execve(target.c_str(), envp, envp) == -1)
	// 		std::exit(0);
	// }
	// else
	// {
	// 	int status;
	// 	close(p_write[0]);
	// 	close(p_read[1]);
	// 	if ((std::size_t)write(p_write[1], body.c_str(), body.size()) < body.size())//attention write peut bloquer set up les pipe en non bloquant d'abord avec fcntl peut etre?
	// 		throw (std::exception());
	// 	read(p_read[0], &buff[0], 8196);//mettre le read en non bloquant sinon les problemes;
	// 	waitpid(pid, &status, 0); //WNOHANG option pour etre non bloquant;
	// 	close(p_write[1]);
	// 	close(p_read[0]);//si waitpid non bloquant
	// 	// std::string response = parse_response(&buff[0]);
	// }
	write(fd, "POST received\n", 15);
	return (1);
}

std::string	Post::type()
{
	return ("POST");
}

Post::~Post()
{

}