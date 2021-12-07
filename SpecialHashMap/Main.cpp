#include <iostream>
#include "SpecialHashMap.h"

int main()
{
	SpecialHashMap<std::string> map;
	map["(5, 2)"] = "value1";
	map["(3, 4)"] = "value2";
	map.toSortedSet();
	std::cout << map.iloc(0) << "\n" << map.ploc(">3 <3") << "\n";
}