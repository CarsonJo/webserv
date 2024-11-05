#include "Http_connection.hpp"

Http_connection::Http_connection() : connect(0), req(0)
{

}

Http_connection::Http_connection(struct pollfd* connect, Request* req): connect(connect), req(req)
{

}

Http_connection::~Http_connection()
{
	std::cout << "call" <<std::endl;
	delete req;
}

struct pollfd*	Http_connection::get_pollfd() const
{
	return (connect);
}

Request*	Http_connection::get_request() const
{
	return (req);
}

void	Http_connection::set_request(Request* req)
{
	if (this->req)
		delete this->req;
	this->req = req;
}