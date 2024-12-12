#include "Delete.hpp"
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include "Date.hpp"
#include "Error.hpp"

Delete::Delete() : Request()
{

}


int Delete::response(int fd)
{
	
    std::cerr << "Handling DELETE request for target: " << target << std::endl;

   
    if (err)
        return err.trap_card_activate();

	//verifier si la methode est autorisee
     if (!(route.get_methods() & DELETE))
	{
        return Error::handle_error(fd, serv, "405", 405); 
	}
	if (!(serv->get_protocol() & DELETE))
	{
        return Error::handle_error(fd, serv, "405", 405); 
	}

    
    if (is_directory(target))
    {
        return Error::handle_error(fd, serv, "403", 403);
    }

    if (access(target.c_str(), F_OK) != 0)
    {
        return Error::handle_error(fd, serv, "404", 404); 
    }

    if (unlink(target.c_str()) != 0)
    {
        return Error::handle_error(fd, serv, "403", 403); // Check si 403 exist
    }

    
    std::string response = "HTTP/1.1 204 No Content\r\n";
    response.append(Date().get_date()).append("\r\n\r\n");

    write(fd, response.c_str(), response.size());
	
    return 1; 
}

std::string	Delete::type()
{
	return ("DELETE");
}

Delete::~Delete()
{

}