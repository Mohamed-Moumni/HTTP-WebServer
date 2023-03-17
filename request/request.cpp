#include <iostream>
#include <fstream>
#include "request.class.hpp"
#include "../utils/utils.hpp"

int get_request_line(request &request)
{
    int i = 0;
    std::vector<std::string> start_line;

    while(request.request_string[i] && request.request_string.substr(i, 2) != "\r\n" && request.request_string[i] != '\n')
        i++;
    start_line = str_split_spaces(request.request_string.substr(0, i));
    if(start_line.size() != 3)
        return 0;
    if(request.request_string[i+1])
        request.request_string = request.request_string.substr(i + 2, request.request_string.size() - 1);
    request.method = start_line[0];
    request.request_target = start_line[1];
    request.http_version = start_line[2];
    return 1;
}

int get_request_headers(request &request)
{
    std::vector<std::string> header_lines;
    std::vector<std::string> key_value;
    std::string header_string;
    //TODO: i have to search not only for \r\n but also for \n because the http message have to work with \n without \r
    if(request.request_string.find("\r\n\r\n") != std::string::npos)
    {
        header_string = request.request_string.substr(0, request.request_string.find("\r\n\r\n"));
        request.request_string = request.request_string.substr(request.request_string.find("\r\n\r\n") + 4);
    }
    else
        return 0;
    header_lines = str_split(header_string, "\r\n");
    for(int i = 0; i < header_lines.size(); i++)
    {
        key_value.clear();
        key_value = header_spliter(header_lines[i]);
        std::cout << key_value.size() << std::endl;
        if(key_value.size() != 2)
            return 0;
        request.headers_map[key_value[0]] = key_value[1];    
    } 
    return 1;
}

int get_request_body(request &request)
{
    request.request_body = request.request_string;
    return 1;
}

int pars_request(request &request)
{
    if(!get_request_line(request) && !get_request_headers(request) && !get_request_body(request))
        return 0;
    if(!get_request_headers(request))
        return 0;
    if(!get_request_body(request))
        return 0;
    return 1;
}

int_fast8_t request_handler(request &request)
{
    if(!pars_request(request))
        return 0;
    return 1;
}

int main()
{
    request request;
    std::string response;
    request.request_string  = "GET / HTTP/1.1\r\nHostlocalhost\r\nConnection: close\r\n\r\nhello everybody here is the body";
    // request.request_string = "GET /index.html HTTP/1.1\r\nHost: localhost:8080\r\nUser-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10.14; rv:94.0) Gecko/20100101 Firefox/94.0\r\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,*/*;q=0.8\r\nAccept-Language: en-US,en;q=0.5\r\nConnection: keep-alive\r\nUpgrade-Insecure-Requests: 1\r\nSec-Fetch-Dest: document\r\nSec-Fetch-Mode: navigate\r\nSec-Fetch-Site: none\r\nSec-Fetch-User:       ?1\r\n\r\nHEY EVERY BODY THE BODY IS HEREE";

    // std::cout << request.request_string << std::endl;
    // request.request_string  = "1 2 3\r\n";
    std::fstream out_file;
    out_file.open("out.html");
    response = "<h1>Parsing finished!</h1>";
    if(!request_handler(request))
    {
        response = bad_request;
        std::cout << "request error" << std::endl;
        return 0;
    }
    out_file << response << std::endl;
    // std::cout << "|||" << request.http_version <<"|||" << std::endl;
    // std::cout << "|||" << request.method <<"|||" << std::endl;
    // std::cout << "|||" << request.request_target <<"|||" << std::endl;
    // std::cout << "|||" << request.request_string <<"|||" << std::endl;
    // std::cout << request.http_version << std::endl;

    std::cout << "HOST:" << request.headers_map["Host"] << std::endl;
    std::cout << "User-Agent:" << request.headers_map["User-Agent"] << std::endl;
    std::cout << "Accept:" << request.headers_map["Accept"] << std::endl;
    std::cout << "Connection:" << request.headers_map["Connection"] << std::endl;
    std::cout << "+++" << request.request_body << "+++" << std::endl;

    
}