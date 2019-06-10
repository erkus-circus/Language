#include <vector>
#include <iostream>
#include <string>

#include <iomanip>

#include "ActionTree.h"
#include "json.hpp"

using json = nlohmann::json;
using namespace std;

char NN[] = "_node_name";
char CHILD[] = "children";
char TYPE[] = "type";
char ARGS[] = "args";

void StateList::add(string a)
{
	string indent = "";
	for (int i = 0; i < indents; i++)
	{
		indent += "\t";
	}
	list.push_back(indent + a);
}
void StateList::addBack(string a)
{
	string b = list.back();
	list.pop_back();
	list.push_back(b + a);

}
string StateList::toString()
{
	string a = "";
	for (size_t i = 0; i < list.size(); i++)
	{
		a += list[i] + "\n";
	}
	return a;
}

void StateList::operator+=(const StateList& a)
{
	for (size_t i = 0; i < a.list.size(); i++)
	{
		add(a.list[i]);
	}
}

const string BUILTINS = R"(
//////////////
// BUILTINS //
//////////////
#include <iostream>
#include <string>

std::string print(std::string a)
{
	// print put a string.
	std::cout << a << std::endl;
	return a;
}

int cmd(std::string a)
{
	// execute a system command.
	return system(a.c_str());
}

////////////
// SCRIPT //
////////////
)";


StateList generateTemplate(json& temp)
{
	StateList list;

	if (temp[NN] == "program")
	{
		list.add(BUILTINS);
	}
	for (size_t i = 0; i < temp[CHILD].size(); i++)
	{
		json node = temp[CHILD][i];

		if (node[NN] == "call")
		{
			// user is calling a function

			// get and add name
			string name = node["name"];
			list.add(name + "(");

			// add arguments
			for (size_t j = 0; j < node[CHILD][CHILD].size(); j++)
			{
				if (j > 0)
				{
					// if not first argument add a comma
					list.addBack(",");
				}

				// get types and values
				string type = node[CHILD][CHILD][j][NN];
				string value = node[CHILD][CHILD][j]["value"];

				// add types and values
				list.addBack(value + "/*@" + type + "*/");
			}

			// close arg list and end line
			list.addBack(");");
		}
		else if (node[NN] == "function")
		{
			string type = node[TYPE];
			list.add(type);

			string name = node[CHILD][0][NN];

			list.addBack(" " + name + "()\n{");

			// add an indent
			list.indents++;

			// add content of function
			json newTemp = node[CHILD][0];
			list += generateTemplate(newTemp);

			// decrease the indents.
			list.indents--;

			// end function. and add newline between next statement
			list.add("}\n");
		}
		else if (node[NN] == "var_declaration")
		{
			
		}
	}

	return list;
}