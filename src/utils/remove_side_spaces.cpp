#include <iostream>

std::string remove_side_spaces(std::string str)
{
    int start = 0;
    int end = str.size() - 1;

    while(str[start] && std::isspace(str[start]))
        start++;
    while((end >= start) && std::isspace(str[end]))
        end--;
    return str.substr(start, (end - start) + 1);
}

// int main()
// {
//     std::string str = "   hello  ";
//     remove_side_spaces(str);
//     std::cout << "|||" << str <<  "|||" << std::endl;
// }