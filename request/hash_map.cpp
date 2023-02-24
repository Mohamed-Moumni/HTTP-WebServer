#include <iostream>

class HashMap {
	public:
		char key;
		int value;
		HashMap *next;

	public:
		HashMap(char _key, int _value) : key(_key), value(_value), next(nullptr)
	{}

};

int main()
{
	HashMap a('a', 10);
	HashMap b('b', 20);
	HashMap e('e', 50);
	HashMap c('c', 30);
	HashMap d('d', 40);
	a.next = &b;
	b.next = &e;
	e.next = &c;
	c.next = &d;

	HashMap* ptr = &a;
	while (ptr != nullptr)
	{
		std::cout << ptr->key << " " << ptr->value << std::endl;
		ptr = ptr->next;
	}
	// std::cout << ptr->next->key << " " << ptr->next->value << std::endl;
	// std::cout << a.next->key << " " << a.next->value << std::endl;
}
