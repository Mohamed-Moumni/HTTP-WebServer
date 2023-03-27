#include <iostream>
#include <vector>
#include "utils.hpp"

std::vector<std::string> header_spliter(std::string header_line)
{
    std::vector<std::string> ret;
    int i = 0;

    while(header_line[i] && header_line[i] != ':')
        i++;

    ret.push_back(header_line.substr(0, i));
    if(header_line[i+1])
        ret.push_back(remove_side_spaces(header_line.substr(i + 1, header_line.size() - 1)));
    return ret;
}
