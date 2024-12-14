#include "Post.hpp"

Post::Post() : Request()
{

}

int	Post::response(int fd)
{
	if (err)
		return (err.trap_card_activate());
	if (!first)
	{
		if (!(serv->get_protocol() & POST))
			return (Error::handle_error(fd, serv, "405", 405));
		if (route.is_cgi_enabled())
		{
			method = CGI;
			return (set_up_cgi(fd));
		}
		else if (route.get_upload_path().size() != 0 && target.find(route.get_upload_path()) != std::string::npos)
			method = UPLOAD;
		else
			return (Error::handle_error(fd, serv, "400", 400));
	}
	else
	{
		switch(method) {
			case CGI :
				return (cgi_handler(fd));
				break;
			case UPLOAD :
				// upload_handler();
				break;
		}
	}
	return (1);
}

std::string	Post::type()
{
	return ("POST");
}

int	Post::type_code()
{
	return (POST);
}

Post::~Post()
{

}