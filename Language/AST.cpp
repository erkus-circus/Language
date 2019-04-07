#include <iostream>
#include <vector>
#include "AST.h"
#include "Lexer.h"
#include "json.hpp"

using namespace std;
using json = nlohmann::json;

json node(string name)
{
	string a = "{\"Anode_name\":\"" + name + "\",\"children\":[]}";
	return json::parse(a);
}

json parse(LexList* lex, string name)
{
	json n = node(name);

	while (lex->getType() != "EOF" || lex->canRetrieve()) //reader is not at End of FILE
	{
		if (!lex->canRetrieve() && lex->getType() == "EOF")
		{
			break;
		}
		lex->skipSpace();

		if (lex->getType() == "STATEMENT")
		{
			string sval = lex->getVal();
			lex->stepUp();
			if (lex->getVal() == ":")
			{
				if (sval == "call")
				{
					cout << "parsing call." << endl;
					n["children"].push_back(parseCall(lex));
				}
			}
		}
		else
		{
			cout << "nope: " << lex->getVal() << endl;
		}
		lex->stepUp();
	}
	return n;
}

json parseCall(LexList* lex)
{
	json n = node("call");
	//lex->stepDown();

	bool gotName = 0;
	bool gotArgs = 0;
	while (lex->canRetrieve() || !gotArgs)
	{
		lex->stepUp();
		lex->skipSpace();
		cout << "tokenVal: " << lex->getVal() << endl;
		while (!gotName)
		{
			if (lex->getType() == "SPACE" || lex->getVal() == ",")
			{
				gotName = 1;
			}
			else
			{
				n["name"] = lex->getVal();
			}
			lex->stepUp(); 
		}

		if (!gotArgs && gotName)
		{
			cout << "parsing args and tval: " << lex->getVal() << endl; 
			n["children"] += parseArgs(lex);
			gotArgs = 1;
		}
	}
	return n;
}
json parseString(LexList* lex)
{
	json n = node("string");
	bool esc = 0;
	while (lex->canRetrieve())
	{
		if (!lex->canRetrieve() || lex->getType() == "EOF")
		{
			break;
		}
		if (!esc && lex->getVal() == "\\")
		{
			esc = 1;
			continue;
		}
		else if (esc)
		{
			esc = 0;
			n["value"] += lex->getVal();
			continue;
		}
		else if (lex->getType() == "STRSEP")
		{
			cout << "breaking" << endl;
			break;
		}
		else
		{
			cout << "adding char: " << lex->getVal() << endl;
			n["value"] += lex->getVal();
		}
		lex->stepUp();
	}
	return n;
}


json parseArgs(LexList* lex, string sep, string end)
{
	json n =  node("arg");
	while (lex->canRetrieve() && lex->getVal() != end)
	{
		lex->skipSpace();
		if ((!lex->canRetrieve() && lex->getType() == "EOF") || lex->getVal() == end)
		{
			break;
		}
		else if (lex->getType() == "STRSEP")
		{
			lex->stepDown();
			cout << "getting string" << endl;
			n["children"] += parseString(lex);
		}
		lex->stepUp();
	}
	return n;
}