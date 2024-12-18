#include "Error.hpp"
#include <map>
#include <string>
#include <iostream>
#include <unistd.h>
#include <sstream>
// for write


	std::string intToString(int value) {
    std::ostringstream oss;
    oss << value;
    return oss.str();
}


std::string generateErrorPage(const std::string& title, const std::string& message) {
    return "<!doctype html>\n"
           "<html>\n"
           "<head>\n"
           "    <title>" + title + "</title>\n"
           "</head>\n"
           "<body>\n"
           "    <h1>" + title + "</h1>\n"
           "    <p>" + message + "</p>\n"
           "</body>\n"
           "</html>\n";
}

std::string e406(const VirtualServ* serv, std::string& header) {
    (void)serv;
    (void)header;
	return generateErrorPage("406 Not Acceptable", "The server cannot produce a response matching the request.");
}

std::string e405(const VirtualServ* serv, std::string& header) {
    std::string methods = "Allowed Methods: ";
    int prot = serv->get_protocol();
    if (prot & GET) methods.append("GET ");
    if (prot & POST) methods.append("POST ");
    if (prot & DELETE) methods.append("DELETE ");
	header += methods;
	(void) serv;
    (void)header;
	return generateErrorPage("405 Method Not Allowed", "The method is not allowed.\n");
}

std::string e404(const VirtualServ* serv, std::string& header) {
    (void)serv;
    (void)header;
	return generateErrorPage("404 Not Found", "The requested resource was not found on this server.");
}

std::string e403(const VirtualServ* serv, std::string& header) {
    (void)serv;
    (void)header;
	return generateErrorPage("403 Forbidden", "Access to the requested resource is forbidden.");
}

std::string e413(const VirtualServ* serv, std::string& header) {
    (void)serv;
    (void)header;
	return generateErrorPage("413 Payload Too Large", "The request is larger than the server is willing or able to process.");
}

std::string e500(const VirtualServ* serv, std::string& header) {
    (void)serv;
    (void)header;
	return generateErrorPage("500 Internal Server Error", "The server encountered an internal error and could not complete your request.");
}

std::string e411(const VirtualServ* serv, std::string& header) {
    (void)serv;
    (void)header;
	return generateErrorPage("411 Length required", "the content length is required.");
}

// std::string e413(const VirtualServ* serv, std::string& header) {
//     (void)serv;
//     (void)header;
// 	return generateErrorPage("413 Content too large", "The content are too large");
// }

std::string e400(const VirtualServ* serv, std::string& header) {
    (void)serv;
    (void)header;
	return generateErrorPage("400 Bad Request", "The server could not understand the request due to invalid syntax.");
}

std::string e301(const VirtualServ* serv, std::string& header) {
    (void)serv;
    (void)header;
	return generateErrorPage("301 Moved Permanently", "The ressources have been moved.");
}

std::map<int, Error_function> fill_function() {
    std::map<int, Error_function> ret;
	ret[301] = e301;
    ret[405] = e405;
    ret[403] = e403;
    ret[404] = e404;
    ret[406] = e406;
    ret[413] = e413;
    ret[500] = e500;
    ret[400] = e400;
    return ret;
}

std::map<int, std::string> status_messages;
void fill_status_messages() {
    status_messages[400] = "Bad Request";
    status_messages[403] = "Forbidden";
    status_messages[404] = "Not Found";
    status_messages[405] = "Method Not Allowed";
    status_messages[406] = "Not Acceptable";
    status_messages[413] = "Payload Too Large";
	status_messages[411] = "Content Length required";
    status_messages[500] = "Internal Server Error";
	status_messages[301] = "Moved Permanently";
}




Error::Error() : fd(-1), serv(NULL), error(0), str_error("") {}

Error::Error(int fd, const VirtualServ* serv, int error, std::string str_error)
    : fd(fd), serv(serv), error(error), str_error(str_error) {}

void Error::set_error(int fd, const VirtualServ* serv, std::string str_error, int error) {
    this->fd = fd;
    this->serv = serv;
    this->error = error;
    this->str_error = str_error;
}

std::string Error::get_error(int code, const VirtualServ* serv, std::string& header) {
    return function_arr.at(code)(serv, header);
}

int Error::trap_card_activate() {
    if (!*this)
        throw std::exception();
    return handle_error(fd, serv, str_error, error);
}


int Error::handle_error(int fd, const VirtualServ* serv, std::string error_code, int error) {


	 if (status_messages.empty()) {
        fill_status_messages();
    }

    std::string reason_phrase = status_messages[error];
	std::string	header = "";

    std::string body = Error::get_error(error, serv, header);


    std::string response;
    response.append("HTTP/1.1 ").append(error_code).append(" ").append(reason_phrase).append("\r\n");
    response.append("Content-Type: text/html\r\n");
    response.append("Content-Length: ").append(intToString(body.size())).append("\r\n");
    response.append(header).append("\r\n");
    response.append(body);


    std::cerr << "ERROR RESPONSE:\n" << response << std::endl;
    write(fd, response.c_str(), response.size());
    return 2;
}

Error::operator bool() const {
    return fd != -1;
}


std::map<int, Error_function> Error::function_arr = fill_function();