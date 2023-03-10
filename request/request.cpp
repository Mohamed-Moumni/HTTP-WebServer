#include <iostream>
#include "request.class.hpp"
#include "../utils/utils.hpp"

int get_request_line(request &request)
{
    int i = 0;
    std::vector<std::string> start_line;

    while(request.request_string[i] && request.request_string[i] != '\n')
        i++;
    start_line = str_split_spaces(request.request_string.substr(0, i));
    if(start_line.size() != 3)
        return -1;
    if(request.request_string[i+1])
        request.request_string = request.request_string.substr(i + 1, request.request_string.size() - 1);
    request.method = start_line[0];
    request.request_target = start_line[1];
    request.http_version = start_line[2];
    return 1;
}

int get_request_headers(request &request)
{
    std::vector<std::string> header_lines;
    std::vector<std::string> key_value;

    header_lines = str_split(request.request_string, '\n');
    for(int i = 0; i < header_lines.size(); i++)
    {
        key_value = header_spliter(header_lines[i]);
        // std::cout << key_value[0] << "--->" << key_value[1] << std::endl;
        if(key_value.size() != 2)
            return -1;
        request.headers_map[key_value[0]] = key_value[1];    
    }

    // for(int i = 0; i < header_lines.size(); i++)
        // std::cout << request.headers_map[key_value[0]] << "--->" << key_value[1] << std::endl;    
    return 1; 
}

int pars_request(request &request)
{
    if(!get_request_line(request))
        return -1;
    if(!get_request_headers(request))
        return -1;
    
    return 1;
}

int request_handler(request &request)
{
    if(!pars_request(request))
        return -1;
    return 1;
}

int main()
{
    request request;
    request.request_string  = "GET /index.html HTTP/1.1\nHost: localhost:8080\nUser-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10.14; rv:94.0) Gecko/20100101 Firefox/94.0\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,*/*;q=0.8\nAccept-Language: en-US,en;q=0.5\nAccept-Encoding: gzip, deflate \\ \nConnection: keep-alive\nUpgrade-Insecure-Requests: 1\nSec-Fetch-Dest: document\nSec-Fetch-Mode: navigate\nSec-Fetch-Site: none\nSec-Fetch-User: ?1\r\n\r\n";
    // std::cout << request.request_string << std::endl;
    // request.request_string  = "trap";
    request_handler(request);
}