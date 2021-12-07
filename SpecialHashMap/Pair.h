#include <string>

class Pair
{
public:
	std::string op;
	double value;

	Pair();
	Pair(std::string op_, double value_);
};

Pair::Pair()
{
	op = "";
	value = 0;
}

Pair::Pair(std::string op_, double value_)
{
	op = op_;
	value = value_;
}