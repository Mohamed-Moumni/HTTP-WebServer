#include <iostream>
#include <vector>
#include <sstream>

using namespace std;

void    HexToDec(const std::string hexValue, size_t & result)
{
    std::stringstream ss;
    ss << std::hex << hexValue;
    ss >> result;
}

int main(void)
{
    size_t res;
    HexToDec(std::string("E"), res);
    cout << res << endl;
    return (0);
}