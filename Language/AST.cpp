//COMMENT LOL IF SYNC TRY THEN PULL TRY THEN FETCH THEN LOLOLOLOL

#include <iostream>
#include <vector>
#include <sstream>
#include "AST.h"
#include "Lexer.h"
#include "json.hpp"

using namespace std;
using json = nlohmann::json;

json node(string name)
{
	string a = "{\"_node_name\":\"" + name + "\",\"children\":[]}";
	return json::parse(a);
}

json parse(LexList& lex, string name,string end)
{
	json n = node(name);

	while (lex.getType() != "EOF" || lex.canRetrieve()) //reader is not at End of FILE
	{
		if (!lex.canRetrieve() && lex.getType() == "EOF")
		{
			break;
		}
		lex.skipSpace();
		if (lex.getVal() == end && name != "program")
		{
			break;
		}
		if (lex.getType() == "STATEMENT")
		{
			string sval = lex.getVal();
			lex.stepUp();
			if (lex.getVal() == ":")
			{
				if (sval == "call")
				{
					n["children"].push_back(parseCall(lex));
				}
				else if (sval == "if")
				{
					n["children"].push_back(parseIf(lex));
				}
				else if (sval == "func")
				{
					n["children"].push_back(parseFunction(lex));

				}
				else if (sval == "var")
				{
					n["children"].push_back(parseVarDec(lex));
				}
			}
		}
		lex.stepUp();
	}
	return n;
}

json parseCall(LexList& lex)
{
	json n = node("call");
	//lex.stepDown();

	bool gotName = 0;
	bool gotArgs = 0;
	while (lex.canRetrieve() || !gotArgs)
	{
		lex.stepUp();
		lex.skipSpace();
		while (!gotName)
		{
			if (lex.getType() == "SPACE" || lex.getVal() == ",")
			{
				gotName = 1;
			}
			else
			{
				n["name"] = lex.getVal();
			}
			lex.stepUp();
		}

		if (!gotArgs && gotName)
		{
			n["children"] += parseArgs(lex);
			gotArgs = 1;
			break;
		}
	}
	return n;
}

json parseString(LexList& lex)
{
	json n = node("string");
	bool esc = 0;
	string val = "";
	while (lex.canRetrieve())
	{
		if (!esc && lex.getVal() == "\\")
		{
			esc = 1;
			lex.stepUp();
			continue;
		}
		else if (esc)
		{
			esc = 0;
			val += lex.getVal();
			lex.stepUp();
			continue;
		}
		else if (lex.getType() == "STRSEP" && !esc)
		{
			break;
		}
		else
		{
			val += lex.getVal();
			//var: EricsFriends @array = "Me", "Myself", "I", "No one else.";
			//cout << val << endl;
		}
		lex.stepUp();
	}
	n["value"] = val;
	return n;
}


json parseArgs(LexList& lex, string sep, string end)
{
	json n =  node("arg");
	while (lex.canRetrieve() && lex.getVal() != end)
	{
		lex.skipSpace();
		if ((!lex.canRetrieve() && lex.getType() == "EOF") || lex.getVal() == end)
		{
			break;
		}
		else if (lex.getType() == "STRSEP")
		{
			lex.stepUp();
			n["children"] += parseString(lex);
		}
		else if (lex.getType() == "NUM" || lex.getType() == "DOT")
		{
			n["children"] += parseNum(lex);
		}
		else if (lex.getType() == "ID")
		{
			string js = ("{\"ID\":\"" + lex.getVal() + "\"}");
			n["children"] += json::parse(js);
		}
		lex.stepUp();
	}
	return n;
}

json parseNum(LexList& lex)
{
	json n = node("num");
	bool dotted = 0;
	string num;
	while (lex.canRetrieve() || !lex.eof())
	{
		if (lex.getVal() == "." && !dotted)
		{
			dotted = true;
			num += ".";
		}
		else if (lex.getVal() == ".")
		{
			cout << "double dot. unexpected ." << endl;
		}
		else if (lex.getType() == "NUM")
		{
			num += lex.getVal();
		}
		else
		{
			break;
		}
		lex.stepUp();
	}
	if (dotted)
	{
		float i;
		stringstream ab(num);
		ab >> i;
		return i;
	}
	else
	{
		int i;
		stringstream ab(num);
		ab >> i;
		return i;
	}
}

json parseIf(LexList& lex)
{
	json n = node("if");
	string left, right, oper;


	return n;
}



json parseVarDec(LexList& lex)
{
	string varName;
	bool gotName = 0, gettingType = 0, gotType = 0, gotEqual = 0;
	string type = "";
	json n = node("var_declaration");

	while (lex.canRetrieve() && !lex.eof())
	{
		lex.skipSpace();
		if (lex.getType() == "ID" && !gotName)
		{
			varName += lex.getVal();
			gotName = 1;
		}
		if (gotName && lex.getVal() == "@")
		{
			gettingType = 1;
		}
		if (gettingType && !gotType && lex.getType() == "ID")
		{
			type += lex.getVal();
			gotType = 1;
		}
		if (gotType && !gotEqual && lex.getVal() == "=")
		{
			gotEqual = 1;
		}
		if (gotEqual)
		{
			// getting arguments also parses type for us. when compiling, an error will occur if more than one value is provided

			n["children"] += parseArgs(lex);
			break;
		}

		lex.stepUp();
	}

	n["name"] = varName;
	n["type"] = type;

	return n;
}


json parseFunction(LexList& lex)
{
	json n = node("function");

	lex.stepUp();

	bool gotName = 0;
	string name = ""; // name of function

	while (lex.canRetrieve() && !lex.eof())
	{
		lex.skipSpace();
		if (lex.getVal() == "{")
		{
			gotName = true;
		}

		if (!gotName)
		{
			name += lex.getVal();
		}
		else
		{
			n["children"] += parse(lex,name,"}");
			break;
		}
		lex.stepUp();
	}
	return n;
}
