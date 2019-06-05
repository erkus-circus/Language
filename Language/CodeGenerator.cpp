#include "CodeGenerator.h"

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include "json.hpp"

using json = nlohmann::json;
using namespace std;

string compile(json js)
{
	vector<string> compiled = {
		"#include <iostream>"
	};

	string comp = "";
	for (size_t i = 0; i < compiled.size(); i++)
	{
		comp += compiled[i] + "\n";
	}

	return comp;
}