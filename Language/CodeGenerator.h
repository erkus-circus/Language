#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include "json.hpp"

using json = nlohmann::json;

class CodeGenerator
{
public:
	CodeGenerator(json node);
private:
	std::string call(json node);
};

