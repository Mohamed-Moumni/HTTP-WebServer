#include <iostream>
#include "request.class.hpp"
#include "../utils/utils.hpp"

int get_request_line(std::string request_string, request &request)
{
    int i = 0;
    std::vector<std::string> start_line;

    while(request_string[i] != '\n')
        i++;
    start_line = str_split_spaces(request_string.substr(0, i));
    if(start_line.size() != 3)
        return -1;
    request.method = start_line[0];
    request.request_target = start_line[1];
    request.http_version = start_line[2];
    return 1;
}
int pars_request(std::string request_string, request & request)
{
    if(!get_request_line(request_string, request))
        return -1;
    std::cout << request.method << std::endl;
    std::cout << request.request_target << std::endl;
    std::cout << request.http_version << std::endl;
    return 1;
}

int request_handler(std::string request_string)
{
    request request;

    if(!pars_request(request_string, request))
        return -1;
    return 1;
}

int main()
{
    std::string request_string  = "GET /index.html HTTP/1.1\nHost: localhost:8080\nUser-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10.14; rv:94.0) Gecko/20100101 Firefox/94.0\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,*/*;q=0.8\nAccept-Language: en-US,en;q=0.5\nAccept-Encoding: gzip, deflate \\ \nConnection: keep-alive\nUpgrade-Insecure-Requests: 1\nSec-Fetch-Dest: document\nSec-Fetch-Mode: navigate\nSec-Fetch-Site: none\nSec-Fetch-User: ?1\r\n\r\n";
    request_handler(request_string);
}