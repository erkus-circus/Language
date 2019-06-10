#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <stdio.h>

#include "Lexer.h"
#include "AST.h"
#include "ActionTree.h"

#include "json.hpp"
using namespace std;
using json = nlohmann::json;

bool endswith(string const& fullString, string const& ending) {
	if (fullString.length() >= ending.length()) {
		return (0 == fullString.compare(fullString.length() - ending.length(), ending.length(), ending));
	}
	else {
		return false;
	}
}

int main(int argc, char* argv[])
{
	if (argc > 1)
	{
		string fileName = argv[1];
		if (!endswith(fileName, ".starp"))
		{
			cout << "Input File needs to end with .STARP (001)" << endl;
			return -1;
		}
		ifstream file(argv[1]);
		if (file.is_open())
		{
			string fileDat, s;
			while (getline(file, s))
			{
				fileDat += "\n" + s;
			}
			LexList lexed = lex(fileDat);
			//lexed.printOut();

			json ast = parse(lexed);

			ofstream outFile("./out.cpp");

			if (outFile.is_open())
			{
				outFile << "/*\n * Star+\n * Eric Diskin\n * V0.5.2" << endl;
				//outFile << "\n" << setw(2) << ast;
				outFile << " */" << endl;
				outFile << "\n" << generateTemplate(ast).toString() << endl;

				outFile.close();

				string out = "";


				cout << "Compiling..." << endl;
				system("g++ ./out.cpp -o sp");
				cout << "Compiled." << endl;

				system("CLS");

				system("sp");
			}
			else
			{
				cout << "Error: output file is unable to be opened. (002)" << endl;
			}

			file.close();
		}
		else
		{
			cout << "File does not exist" << endl;
		}
		return 0;
	}
}