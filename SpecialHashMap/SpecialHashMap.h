#pragma once
#include <algorithm>
#include <regex>
#include <vector>
#include <unordered_map>
#include "Pair.h"

bool check(double key, double condition, std::string op)
{
	if (op == "<")
	{
		return key < condition;
	}
	if (op == ">")
	{
		return key > condition;
	}
	if (op == ">=")
	{
		return key >= condition;
	}
	if (op == "<=")
	{
		return key <= condition;
	}
	if (op == "<>")
	{
		return key != condition;
	}
	if (op == "" || op == "=")
	{
		return key == condition;
	}
	throw std::exception("Unknown boolean operator");
}

template <class V>
class SpecialHashMap : public std::unordered_map<std::string, V>
{
private:
	std::vector <std::string> keys;
public:
	V iloc(const size_t index) const;
	SpecialHashMap<V> ploc(const std::string condition) const;
	void toSortedSet();
	void import(const std::unordered_map<std::string, V>);
	template <class V> friend std::ostream& operator << (std::ostream& out, const SpecialHashMap<V>& map);
};

template <class V>
V SpecialHashMap<V>::iloc(const size_t index) const
{
	if (index >= this->size())
	{
		throw std::exception("Index out of range");
	}
	if (index >= keys.size())
	{
		throw std::exception("You need to sort keys first! Use method .toSortedSet");
	}
	return this->at(keys[index]);
}

template <class V>
SpecialHashMap<V> SpecialHashMap<V>::ploc(const std::string condition) const
{
	SpecialHashMap<V> result;
	std::regex regex_condition("(<|>|<=|>=|=|<>|)[\\s]*([0-9]+\\.?[0-9]*)");
	std::regex regex_discard("[^\\d\\(\\)\\s,\\.]");
	std::regex regex_key("[\\d]+\\.?[\\d]*");

	std::vector <Pair> conditions;
	for (std::sregex_iterator it = std::sregex_iterator(condition.begin(), condition.end(), regex_condition); it != std::sregex_iterator(); it++)
	{
		std::smatch match = *it;
		conditions.push_back(Pair(match[1].str(), std::stod(match[2].str())));
	}
	
	if (conditions.size() == 0)
	{
		throw std::exception("No conditions found!");
	}
	for (auto it = this->begin(); it != this->end(); it++)
	{
		std::vector <double> keys;
		std::string key = it->first;
		if (std::regex_search(key, regex_discard))
		{
			continue;
		}
		for (std::sregex_iterator it = std::sregex_iterator(key.begin(), key.end(), regex_condition); it != std::sregex_iterator(); it++)
		{
			std::smatch match = *it;
			keys.push_back(std::stod(match.str()));
		}
		if (conditions.size() != keys.size())
		{
			continue;
		}
		bool success = true;
		for (size_t i = 0; i < keys.size(); i++)
		{
			if (check(keys[i], conditions[i].value, conditions[i].op))
			{
				continue;
			}
			success = false;
			break;
		}
		if (success)
		{
			result[it->first] = it->second;
		}
	}
	return result;
}

template <class V>
void SpecialHashMap<V>::toSortedSet()
{
	keys.resize(this->size());
	size_t i = 0;
	for (auto it = this->begin(); it != this->end(); it++)
	{
		keys[i++] = it->first;
	}
	std::sort(keys.begin(), keys.end());
}

template <class V>
void SpecialHashMap<V>::import(const std::unordered_map<std::string, V> map)
{
	this->clear();
	for (auto it = map.begin(); it != map.end(); it++)
	{
		this->emplace(it->first, it->second);
	}
}

template <class V>
std::ostream& operator<<(std::ostream& out, const SpecialHashMap<V>& map)
{
	out << "{";
	bool next = false;
	for (auto it = map.begin(); it != map.end(); it++)
	{
		if (next)
		{
			out << ", " << it->first << "=" << it->second;
			continue;
		}
		out << it->first << "=" << it->second;
		next = true;
	}
	out << "}";
	return out;
}