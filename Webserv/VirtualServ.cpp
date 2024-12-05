# include "VirtualServ.hpp"
#include <iostream>
#include <sys/stat.h>

std::map<std::string, ServerParseFunction> VirtualServ::server_elem = VirtualServ::init_static_elem();

std::string	get_value(const std::string& line, int (*f)(int))
{
	int			j = 0;
	std::string	sub;
	std::size_t	pos = line.find_first_not_of(" \t");
	std::size_t	temp = pos;

	if (pos == std::string::npos)
		throw(std::exception());
	while (f(line[pos + j]) && pos + j < line.size())
		j++;
	if (pos + j >= line.size())
		throw(std::exception());
	sub = line.substr(pos + j);
	pos = sub.find_first_not_of(" \t");
	if (pos == std::string::npos || sub[pos] != ';')
		throw(std::exception());
	return (line.substr(temp, j));
}

/*static int myalnum(int c)
{
	if (c != '}' && isalnum(c))
		return (1);
	return (0);
}
*/
static int myascci(int c)
{
	if (c != '}' && c != ';' && isascii(c))
		return (1);
	return (0);
}
static int alnum_path(int c)
{
	if (c == '/' || (c != '}' && isalnum(c)))
		return (1);
	return (0);
}

void	VirtualServ::server_var(const std::string &line, VirtualServ &serv)
{
	std::string	temp = get_value(line, isalnum);
	serv.set_name(temp);
}

void	VirtualServ::listen_var(const std::string &line, VirtualServ &serv)
{
	std::string	temp = get_value(line, isdigit);
	serv.set_port(temp);
}

void	VirtualServ::host_var(const std::string &line, VirtualServ &serv)
{
	std::string	temp = get_value(line, myascci);
	serv.set_host(temp);
}


bool is_directory(const std::string &path) {
    struct stat path_stat;
    stat(path.c_str(), &path_stat);
    return S_ISDIR(path_stat.st_mode);
}

bool is_file(const std::string &path) {
    struct stat path_stat;
    stat(path.c_str(), &path_stat);
    return S_ISREG(path_stat.st_mode);
}

void check_directory_path(const std::string& path) {
    

    std::string temp = "." + path;


   
    if (!is_directory(temp)) {
        throw std::runtime_error("Path is not a valid directory: " + temp);
    }

    
    if (access(temp.c_str(), R_OK | W_OK | X_OK) != 0) {
        throw std::runtime_error("Insufficient permissions for directory: " + temp);
    }
}

void check_file_path(const std::string& path) {
    
    std::string temp = "." + path;

   
    if (!is_file(temp)) {
        throw std::runtime_error("Path is not a valid file: " + temp);
    }


    if (access(temp.c_str(), R_OK | W_OK) != 0) {
        throw std::runtime_error("Insufficient permissions for file: " + temp);
    }
}

void	VirtualServ::root_var(const std::string& line, VirtualServ &serv)
{
	
	std::string	temp = get_value(line, alnum_path);
	std::cout << "entering root- " << std::endl;
	check_directory_path(temp);
	serv.set_root(temp);
	
}

void	VirtualServ::default_var(const std::string& line, VirtualServ& serv)
{
	std::string	temp = get_value(line, myascci);
		check_file_path(temp);
		serv.set_default(temp);
	
}

void	VirtualServ::protocol_var(const std::string& line, VirtualServ& serv)
{
	std::string	temp = get_value(line, myascci);
	if (temp.find("GET") != std::string::npos)
		serv.accepted_protocol |= GET;
	if (temp.find("POST") != std::string::npos)
		serv.accepted_protocol |= POST;
	if (temp.find("DELETE") != std::string::npos)
		serv.accepted_protocol |= DELETE;
	std::cout << "protocole : " << serv.accepted_protocol << std::endl;
}

std::map<std::string, ServerParseFunction> VirtualServ::init_static_elem()
{
	std::map<std::string, ServerParseFunction> ret;

	ret["server_name"] = VirtualServ::server_var;
	ret["listen"] = VirtualServ::listen_var;
	ret["root"] = VirtualServ::root_var;
	ret["default"] = VirtualServ::default_var;
	ret["protocol"] = VirtualServ::protocol_var;
	ret["host"] = VirtualServ::host_var;
	return (ret);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

VirtualServ::VirtualServ() : port(""), server_name(""), root(""), host ("0.0.0.0"), accepted_protocol(0)
{

}



VirtualServ::~VirtualServ()
{

}

// void	VirtualServ::operator=(VirtualServ& to_copy)
// {
// 	socket_fd = to_copy.socket_fd;
// 	connection_fd = to_copy.connection_fd;
// 	port = to_copy.port;
// 	server_name = to_copy.server_name;
// }

//verifier ce qui se passe si fail de construction de socket_fd

void	VirtualServ::set_default(const std::string& line)
{
	default_page = line;
}


void	VirtualServ::set_name(const std::string &name)
{
	this->server_name = name;
}

void	VirtualServ::set_host(const std::string &host)
{
	this->host = host;
}

void	VirtualServ::set_port(const std::string &name)
{
	this->port = name;
}

void	VirtualServ::set_root(const std::string &root)
{
	this->root = root;
}

std::string	VirtualServ::get_root() const
{
	return (root);
}

std::string	VirtualServ::get_name() const
{
	return (server_name);
}

std::string	VirtualServ::get_port() const
{
	return (port);
}

std::string	VirtualServ::get_host() const
{
	return (host);
}

int VirtualServ::get_protocol() const
{
	return (accepted_protocol);
}


std::map<std::string, ParseFunction> Route::route_directives = Route::init_route_directives();

Route::Route() : autoindex(false), allowed_methods(0), cgi_enabled(false) {}
Route::~Route() {}

void Route::set_location(const std::string& loc) { location = loc; }
void Route::set_root(const std::string& root) { this->root = root; }
void Route::set_default(const std::string& default_page) { this->default_page = default_page; }
void Route::set_autoindex(bool index) { autoindex = index; }
void Route::set_methods(int methods) { allowed_methods = methods; }

void Route::set_cgi(bool enabled) {
    cgi_enabled = enabled;
}

void Route::set_upload_path(const std::string& path) {
    upload_path = path;
}

bool Route::is_cgi_enabled() const {
    return cgi_enabled;
}

const std::string& Route::get_upload_path() const {
    return upload_path;
}

const std::string& Route::get_location() const { return location; }
const std::string& Route::get_root() const { return root; }
const std::string& Route::get_default() const { return default_page; }
bool Route::is_autoindex() const { return autoindex; }
int Route::get_methods() const { return allowed_methods; }

std::map<std::string, ParseFunction> Route::init_route_directives() {
    std::map<std::string, ParseFunction> ret;

    ret["location"] = Route::location_var;
    ret["root"] = Route::root_var;
    ret["autoindex"] = Route::autoindex_var;
    ret["methods"] = Route::methods_var;
	ret["default"] = Route::methods_var;
	ret["cgi"] = Route::cgi_var;
    ret["upload_path"] = Route::upload_path_var;

    return ret;
}

void Route::location_var(const std::string& line, Route& route) {
    route.set_location(get_value(line, alnum_path));
}

void Route::root_var(const std::string& line, Route& route) {
  	std::string	temp = get_value(line, alnum_path);
	check_directory_path(temp);
	route.set_root(temp);
	}


void Route::default_var(const std::string& line, Route& route) {
  
		std::string	temp = get_value(line, myascci);
		check_file_path(temp);
		route.set_default(temp);
	
}

void Route::autoindex_var(const std::string& line, Route& route) {
    std::string temp = get_value(line, isalnum);
    route.set_autoindex(temp == "on");
}

void Route::methods_var(const std::string& line, Route& route) {
    std::string temp = get_value(line, myascci);
    if (temp.find("GET") != std::string::npos)
		route.allowed_methods |= GET;
	if (temp.find("POST") != std::string::npos)
		route.allowed_methods  |= POST;
	if (temp.find("DELETE") != std::string::npos)
		route.allowed_methods  |= DELETE;
}


void VirtualServ::add_route(const Route& route) {
     routes[route.get_location()] = route; 
}


const std::map<std::string, Route>& VirtualServ::get_routes() const {
    return routes; 
}

void Route::cgi_var(const std::string& line, Route& route) {
    std::string temp = get_value(line, myascci);
    route.set_cgi(temp == "on");
}

void Route::upload_path_var(const std::string& line, Route& route) {
    std::string temp = get_value(line, alnum_path);
    check_directory_path(temp);
    route.set_upload_path(temp);
}