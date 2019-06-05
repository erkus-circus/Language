#include <iostream>
#include <iomanip>
#include <fstream>

#include "Lexer.h"
#include "AST.h"
#include "CodeGenerator.h"

#include "json.hpp"
// var: name@type = (ID|STR|INT);
using namespace std;
using json = nlohmann::json;

int main()
{
	cout << "Language [V0.4.1] \nEric Diskin 2019" << endl;
	// TODO: make ifs

	string a = "";
	string all = "";
	json ab;
	while (a != "~")
	{
		cout << ">>>";
		getline(cin, a);
		string lchar = { a.back() };
		if (a == "1")
		{
			all = "";
			continue;
		}
		if (lchar == "\\")
		{
			a.pop_back();
			all += "\n" + a;
			continue;
		}
		all += "\n" + a;
		
		LexList lexed = lex(all);
		//lexed.printOut();

		json ast = parse(lexed);

		cout << std::setw(4) << ast << endl;
	}
}