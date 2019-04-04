#include <iostream>
#include <fstream>

#include "Lexer.h"
#include "AST.h"

using namespace std;

int main()
{
	cout << "Lexer [V1.1] \nEric Diskin 2019" << endl;

	string a;

	while (a != "~") {
		cout << ":>";
		getline(cin, a);

		LexList l = lex(a);

		l.printOut();

		//parse(&l)->printOut();
	}
}