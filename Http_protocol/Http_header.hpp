#ifndef HTTP_HEADER_HPP
#define HTTP_HEADER_HPP
#include "Request.hpp"
#include "../Webserv/ServerBlock.hpp"

typedef void (*Parsed_header)(Request*, std::string& toRead, std::size_t& pos);

class Http_header
{
	public :

		static void	location(Request *req, std::string& toRead, std::size_t& pos, ServerBlock *serv);
		static void	content_length(Request *req, std::string& toRead, std::size_t& pos, ServerBlock *serv);
		static void	content_type(Request *req, std::string& toRead, std::size_t& pos, ServerBlock *serv);
		static void	status(Request *req, std::string& toRead, std::size_t& pos, ServerBlock *serv);
		static void	authorization(Request *req, std::string& toRead, std::size_t& pos, ServerBlock *serv);
		static void	host(Request *req, std::string& toRead, std::size_t& pos, ServerBlock *serv);

	private :
		Http_header();

};
#endif