#include "../INCLUDES/request.hpp"

std::string respond_error(std::string error, ConfigFile configfile)
{
    std::ostringstream body;
    std::ostringstream resp;
    body <<  "<h1 style=\"text-align: center\">" << error << " " << configfile._code_status[error] << "<h1>";
    resp << "HTTP/1.1 " << error << " " <<configfile._code_status[error] << CRLF << "Content-Type: text/html" << CRLF << "Content-Length: " << body.str().size() << CRLF << CRLF;
    resp << body.str();
    return resp.str();
}