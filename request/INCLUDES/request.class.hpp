#pragma once

#include <iostream>
#include <map>

// std::string bad_request = "HTTP/1.1 400 Bad Request\r\nServer: 42_WEBSERVER\r\nContent-Type: text/html\r\nContent-Length: 141\r\nConnection: close\r\n\r\n<html><head><title>400 Bad Request</title></head><body><center><h1>400 Bad Request</h1></center><hr><center>42_WEBSERV</center></body></html>";
// std::string not_allowed = "";

class request
{ 
    public:
        size_t      ContentLen;
        size_t      BodyReaded;
        std::string request_string;
        std::string method;
        std::string http_version;
        std::string request_target;
        std::string original_request_target;
        std::map<std::string , std::string> headers_map;
        std::string request_body;
};