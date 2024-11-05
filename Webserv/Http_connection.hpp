#ifndef HTTP_CONNECTION_HPP
# define HTTP_CONNECTION_HPP
#include "../Http_protocol/Request.hpp"

class Http_connection{

	private :

		struct pollfd*	connect;
		Request*		req;

	public :

		Http_connection();
		Http_connection(struct pollfd* connect, Request* req);
		~Http_connection();
		struct pollfd*	get_pollfd() const;
		void			set_request(Request* req);
		Request*		get_request() const;

};
#endif