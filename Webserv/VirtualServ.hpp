#ifndef SERVER_HPP
# define SERVER_HPP
# include <string>
# include <map>
# include <fstream>
# include <exception>
# include <vector>
# include <algorithm>
# include <poll.h>
# include <unistd.h>
# include "lib_function.hpp"
# include "Route.hpp"
# include "Addrinfo.hpp"
# include "Fsocket.hpp"
// # define REP_SERVER "/home/cjozefzo/Documents/cursus42/webserv"

class VirtualServ;

class Route;

void config_server(std::fstream &config, std::string &line, VirtualServ &server);
typedef void (*ServerParseFunction)(const std::string& line, VirtualServ& serv);

class VirtualServ {
public:
    VirtualServ();
    ~VirtualServ();


    void set_name(const std::string& name);
    void set_root(const std::string& root);
    void set_default(const std::string& page);
    void set_port(const std::string& port);
	void set_host(const std::string& host);


    std::string get_name() const;
    std::string get_port() const;
	std::string get_host() const;
    std::string get_root() const;
    int get_protocol() const;


    static void parse_route(std::fstream& config, std::string& line, VirtualServ& server);
    static void server_var(const std::string& line, VirtualServ& serv);
    static void listen_var(const std::string& line, VirtualServ& serv);
    static void root_var(const std::string& line, VirtualServ& serv);
    static void default_var(const std::string& line, VirtualServ& serv);
	static void host_var(const std::string& line, VirtualServ& serv);
    static void protocol_var(const std::string& line, VirtualServ& serv);
    static std::map<std::string, ServerParseFunction> init_static_elem();
    static std::map<std::string, ServerParseFunction>	server_elem;


    void add_route(const Route& route);
    const std::map<std::string, Route>& get_routes() const;

private:
    std::string port;
    std::string server_name;
    std::string root;
	std::string host;
    std::string default_page;
    int accepted_protocol;

    std::map<std::string, Route> routes;

};

#endif
