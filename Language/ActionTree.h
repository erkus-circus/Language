#pragma once

#include <vector>
#include <iostream>
#include <string>

#include "json.hpp"

using json = nlohmann::json;


class StateList
{
public:
	std::vector<std::string> list;
	int indents = 0;
	void add(std::string a);
	void addBack(std::string a);
	void operator+=(const StateList&);
	std::string toString();
};

StateList generateTemplate(json& temp);