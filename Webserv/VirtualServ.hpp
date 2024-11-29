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
# include "Addrinfo.hpp"
# include "Fsocket.hpp"
# include "Webserv.hpp"
# include "../Http_protocol/Request.hpp"
// # define REP_SERVER "/home/cjozefzo/Documents/cursus42/webserv"
# define GET 1
# define POST 2
# define DELETE 4

class VirtualServ;

class Route;

void config_server(std::fstream &config, std::string &line, VirtualServ &server);
typedef void (*ParseFunction)(const std::string& line, Route& route);
typedef void (*ServerParseFunction)(const std::string& line, VirtualServ& serv);

class Route {
public:
    Route();
    ~Route();

    void set_location(const std::string& loc);
    void set_root(const std::string& root);
    void set_autoindex(bool index);
    void set_methods(int methods);

    
    const std::string& get_location() const;
    const std::string& get_root() const;
    bool is_autoindex() const;
    int get_methods() const;

   
    static void location_var(const std::string& line, Route& route);
    static void root_var(const std::string& line, Route& route);
    static void autoindex_var(const std::string& line, Route& route);
    static void methods_var(const std::string& line, Route& route);
    static std::map<std::string, ParseFunction> init_route_directives();
    static std::map<std::string, ParseFunction> route_directives;


private:
    std::string location;
    std::string root;
    bool autoindex;
    int allowed_methods;

};

class VirtualServ {
public:
    VirtualServ();
    ~VirtualServ();

   
    void set_name(const std::string& name);
    void set_root(const std::string& root);
    void set_default(const std::string& page);
    void set_port(const std::string& port);

  
    std::string get_name() const;
    std::string get_port() const;
    std::string get_root() const;
    int get_protocol() const;

   
    static void parse_route(std::fstream& config, std::string& line, VirtualServ& server);
    static void server_var(const std::string& line, VirtualServ& serv);
    static void listen_var(const std::string& line, VirtualServ& serv);
    static void root_var(const std::string& line, VirtualServ& serv);
    static void default_var(const std::string& line, VirtualServ& serv);
    static void protocol_var(const std::string& line, VirtualServ& serv);
    static std::map<std::string, ServerParseFunction> init_static_elem();
    static std::map<std::string, ServerParseFunction>	server_elem;
    

    void add_route(const Route& route);
    const std::map<std::string, Route>& get_routes() const; 

private:
    std::string port;
    std::string server_name;
    std::string root;
    std::string default_page;
    int accepted_protocol;

    std::map<std::string, Route> routes;
  
};

#endif
