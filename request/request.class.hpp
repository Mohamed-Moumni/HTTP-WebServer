#include <iostream>
#include <map>

class request
{
    public:
        std::string request_string;
        std::string method;
        std::string http_version;
        std::string request_target;
        std::map<std::string , std::string> headers_map;
};