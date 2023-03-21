#include <iostream>
#include <vector>
#include "utils.hpp"

std::vector<std::string> header_spliter(std::string header_line)
{
    std::vector<std::string> ret(2);
    int i = 0;

    while(header_line[i] && header_line[i] != ':')
        i++;

    ret[0] = remove_side_spaces(header_line.substr(0, i));
    if(header_line[i+1])
        ret[1] = remove_side_spaces(header_line.substr(i + 1, header_line.size() - 1));
    return ret;
}
