#include <iostream>
#include <map>

int main()
{

	std::map<char, int> mp;

	mp.insert(std::make_pair('a', 10));
	mp['b'] = 20;
	mp['d'] = 50;
	mp['c'] = 30;

	std::map<char, int>::iterator it;

	it = mp.begin();
	for (; it != mp.end(); it++)
	{
		std::cout << it->first << "  " << it->second << std::endl;
	}

	return 0;
}
