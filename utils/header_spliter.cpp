#include <iostream>
#include <vector>

std::vector<std::string> header_spliter(std::string header_line)
{
    std::vector<std::string> ret(2);
    int i = 0;

    while(header_line[i] && header_line[i] != '\n')
        i++;
    ret[0] = header_line.substr(0, i);
    if(header_line[i+1])
        ret[1] = header_line.substr(i + 1, header_line.size() - 1);
    std::cout << ret[0] << "->>>" << ret[1] << std::endl;
    return ret;
}