#include <iostream>
#include <map>

std::string bad_request = "";
std::string not_allowed = "";

class request
{
    public:
        std::string request_string;
        std::string method;
        std::string http_version;
        std::string request_target;
        std::map<std::string , std::string> headers_map;
        std::string request_body;
};