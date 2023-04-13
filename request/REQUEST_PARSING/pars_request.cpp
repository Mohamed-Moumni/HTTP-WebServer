#include "../INCLUDES/request.hpp"

int get_request_line(request &request)
{
    int i = 0;
    std::vector<std::string> start_line;

    while(request.request_string[i] && request.request_string.substr(i, 2) != "\r\n")
        i++;
    start_line = str_split_spaces(request.request_string.substr(0, i));
    if(start_line.size() != 3)
        return 0;
    if(request.request_string[i+1])
        request.request_string = request.request_string.substr(i + 2, request.request_string.size() - 1);
    request.method = start_line[0];
    request.request_target = start_line[1];
    request.original_request_target = start_line[1];
    request.http_version = start_line[2];
    return 1;
}

int get_request_headers(request &request)
{
    std::vector<std::string> header_lines;
    std::vector<std::string> key_value;
    std::string header_string;

    if(request.request_string.find("\r\n\r\n") != std::string::npos)
    {
        header_string = request.request_string.substr(0, request.request_string.find("\r\n\r\n"));
        request.request_string = request.request_string.substr(request.request_string.find("\r\n\r\n") + 4);
    }
    else
        return 0;
    header_lines = str_split(header_string, "\r\n");
    for(size_t i = 0; i < header_lines.size(); i++)
    {
        key_value.clear();
        key_value = header_spliter(header_lines[i]);
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
    if(!get_request_line(request))
        return 0;
    if(!get_request_headers(request))
        return 0;
    if(!get_request_body(request))
        return 0;
    return 1;
}
