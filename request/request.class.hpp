#include <iostream>

class request
{
    public:
        //request line
        std::string method;
        std::string http_version;
        std::string request_target;
        //headers
        std::string host;
        unsigned int content_lenght;
        

        
};