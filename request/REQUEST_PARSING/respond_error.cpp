#include "../INCLUDES/request.hpp"



std::string respond_error(std::string error, ConfigFile configfile)
{
    std::ostringstream body;
    std::ostringstream resp;
    // Server server;
    // find_server(socket, configfile, server);
    if(configfile._custom_error_pages.find(error) != configfile._custom_error_pages.end())
        body << configfile._custom_error_pages[error];
    else
        body <<  "<h1 style=\"text-align: center\">" << error << " " << configfile._code_status[error] << "<h1>";
    resp << "HTTP/1.1 " << error << " " <<configfile._code_status[error] << CRLF << "Content-Type: text/html" << CRLF << "Content-Length: " << body.str().size() << CRLF << CRLF;
    resp << body.str();
    return resp.str();
}