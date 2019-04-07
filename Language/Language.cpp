#include <iostream>
#include <fstream>
#include <iomanip>
#include "Lexer.h"
#include "AST.h"

#include "json.hpp"

using namespace std;
using json = nlohmann::json;

int main()
{
	cout << "Lexer & AST Generator [V0.2.1] \nEric Diskin 2019" << endl;

	string example = "call: hi, 'wow';";
	
	LexList lexed = lex(example);
	
	lexed.printOut();
	
	json ast = parse(&lexed);

	cout << setw(4) << ast << std::endl;
}