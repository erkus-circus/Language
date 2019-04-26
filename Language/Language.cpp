#include <iostream>
#include <iomanip>
#include <fstream>
#include "Lexer.h"
#include "AST.h"

#include "json.hpp"

using namespace std;
using json = nlohmann::json;

int main()
{
	cout << "Lexer & AST Generator [V0.2.1] \nEric Diskin 2019" << endl;
	cout << "so far what you can do is run a line like: " << endl;
	cout << "call: ANYNAMETHATISNOTNUMBERS, ARGS_LIKESTRING_OR_NUMBERS;" << endl;
	cout << "call: print, 'lol this is a string and you can use backlash', 12, \" twelve is a number\";";
	cout << "the first thing u see is a list of tokens. the JSON is the Abstract Syntax\
 Tree. type ~ into the command prompt to close." << endl;

	string a = "";
	while (a != "~")
	{
		cout << ">>>";
		getline(cin, a);
		LexList lexed = lex(a);
		lexed.printOut();

		json ast = parse(&lexed);

		cout << std::setw(4) << ast << endl;
	}
}