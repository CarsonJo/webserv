#ifndef MAIN_HPP
# define MAIN_HPP
# define SERVER_BLOCK 2
#include <poll.h>
#include <string>
#include <iostream>
#include <errno.h>
#include "Webserv.hpp"
#include "Addrinfo.hpp"
#include "Fsocket.hpp"
#include "VirtualServ.hpp"
# include "VirtualServ.hpp"
int	parse_config(std::string name, Webserv& server);
#endif