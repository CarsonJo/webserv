#include "Post.hpp"

Post::Post() : Request()
{

}

int	Post::response(int fd)
{
	
	if (err)
	{
			std::cout << "FFSFSFKSMFKSFML entering post" << std::endl;
		return (err.trap_card_activate());

	}
	
	
		std::cout << "entering post" << std::endl;
		if (!(serv->get_protocol() & POST))
			return (Error::handle_error(fd, serv, "405", 405));
		if (route.is_cgi_enabled())
		{
			method = CGI;
			return (set_up_cgi(fd));
		}
		else if (route.get_upload_path().size() != 0)
		{
			std::cout << "entering route" << std::endl;
			method = UPLOAD;

		}
		else
		{
			std::cout << "processing error" << std::endl;
			return (Error::handle_error(fd, serv, "400", 400));
		}
	
	
	
		switch(method) {
			case CGI :
				return (cgi_handler(fd));
				break;
			case UPLOAD :
				return (handle_multipart(fd));
				break;
		}
	
	return (1);
}

int Post::handle_multipart(int fd)
{
    std::cout << "[DEBUG] Starting handle_multipart function." << std::endl;

    // Step 1: Extract the Content-Type from the request
    std::string content_type = get_content_type();
    if (content_type.empty())
    {
        std::cout << "[ERROR] Content-Type is missing in the request." << std::endl;
        return Error::handle_error(fd, get_serv(), "400", 400);
    }

    std::cout << "[DEBUG] Content-Type: " << content_type << std::endl;

    // Step 2: Ensure the Content-Type is multipart/form-data
    if (content_type.find("multipart/form-data") == std::string::npos)
    {
        std::cout << "[ERROR] Content-Type is not multipart/form-data." << std::endl;
        return Error::handle_error(fd, get_serv(), "400", 400);
    }

    // Step 3: Extract the boundary from the request
    std::string boundary = get_boundary();
    if (boundary.empty())
    {
        std::cout << "[ERROR] Boundary value is missing." << std::endl;
        return Error::handle_error(fd, get_serv(), "400", 400);
    }

    boundary = "--" + boundary; // Prepend boundary marker

    std::cout << "[DEBUG] Extracted boundary: " << boundary << std::endl;

    // Step 4: Extract the body of the request
    const std::string& body = get_body();
    if (body.empty())
    {
        std::cout << "[ERROR] Request body is empty." << std::endl;
        return Error::handle_error(fd, get_serv(), "400", 400);
    }

    std::cout << "[DEBUG] Request body size: " << body.size() << std::endl;

    // Step 5: Parse the body using the boundary
    std::string::size_type pos = body.find(boundary);
    while (pos != std::string::npos)
    {
        std::string::size_type end_pos = body.find(boundary, pos + boundary.length());
        if (end_pos == std::string::npos)
        {
            std::cout << "[ERROR] Boundary end not found." << std::endl;
            break;
        }

        std::string part = body.substr(pos + boundary.length(), end_pos - pos - boundary.length());
        pos = end_pos;

        std::cout << "[DEBUG] Processing part: " << part.substr(0, 50) << "..." << std::endl;

       // Step 6: Parse each part for headers and content
std::cout << "[DEBUG] Current part: " << part.substr(0, 100) << "..." << std::endl;

std::string::size_type header_end = part.find("\r\n\r\n");
if (header_end == std::string::npos)
{
    std::cout << "[ERROR] Part headers missing or header delimiter not found." << std::endl;
    continue;
}

std::string headers_part = part.substr(0, header_end);
std::string content_part = part.substr(header_end + 4); // Skip the \r\n\r\n

headers_part.erase(0, headers_part.find_first_not_of(" \t\r\n"));
std::cout << "[DEBUG] Extracted headers part: " << headers_part << std::endl;
std::cout << "[DEBUG] Extracted content part size: " << content_part.size() << std::endl;


// Parse headers
std::istringstream headers_stream(headers_part);
std::string header_line;
std::string disposition;

while (std::getline(headers_stream, header_line))
{
  
    std::cout << "[DEBUG] Parsed header line: " << header_line << std::endl;

    if (header_line.empty())
        break;

    if (header_line.find("Content-Disposition:") != std::string::npos)
    {
        disposition = header_line;
        std::cout << "[DEBUG] Found Content-Disposition: " << disposition << std::endl;
    }
}

// Validate Content-Disposition
if (disposition.empty() || disposition.find("form-data") == std::string::npos)
{
    std::cout << "[ERROR] Invalid or missing Content-Disposition header." << std::endl;
    continue;
}

        // Extract the filename (if present)
        std::string::size_type filename_pos = disposition.find("filename=");
        if (filename_pos != std::string::npos)
        {
            filename_pos += 9; // Move past "filename="
            std::string::size_type filename_end = disposition.find("\"", filename_pos + 1);
            std::string filename = disposition.substr(filename_pos + 1, filename_end - filename_pos - 1);

            std::cout << "[DEBUG] Extracted filename: " << filename << std::endl;

            // Step 7: Save the file to the destination defined in the config
            std::string upload_path = route.get_upload_path();
            if (upload_path.empty())
            {
                std::cout << "[ERROR] Upload path is not set." << std::endl;
                return Error::handle_error(fd, get_serv(), "500", 500);
            }

            std::ofstream outfile((upload_path + "/" + filename).c_str(), std::ios::binary);
            if (!outfile)
            {
                std::cout << "[ERROR] Failed to open file for writing: " << upload_path + "/" + filename << std::endl;
                return Error::handle_error(fd, get_serv(), "500", 500);
            }

            outfile.write(content_part.c_str(), content_part.size());
            outfile.close();
            std::cout << "[DEBUG] File saved successfully: " << filename << std::endl;
        }
    }

    std::cout << "[DEBUG] handle_multipart function completed successfully." << std::endl;
    return 1; // Success
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