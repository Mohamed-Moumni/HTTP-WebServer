#include "../INCLUDES/request.hpp"

std::string respond_error(std::string error, ConfigFile configfile)
{
    std::ostringstream body;
    body <<  "<h1 style> " << error << " " << configfile._error_pages[error] << "<h1>"
    return configfile._error_pages[error];
}