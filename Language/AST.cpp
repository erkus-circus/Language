//COMMENT LOL IF SYNC TRY THEN PULL TRY THEN FETCH THEN LOLOLOLOL

#include <iostream>
#include <vector>
#include <sstream>

#include <iomanip>
#include "AST.h"
#include "Lexer.h"
#include "json.hpp"

#define NN "_node_name"
using namespace std;
using json = nlohmann::json;

json node(string name)
{
	json b;
	b[NN] = name;
	b["children"] = json::array();
	return b;
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
				else if (sval == "return")
				{
					n["children"].push_back(parseReturn(lex));
				}
			}
		}
		lex.stepUp();
	}
	return n;
}

json parseReturn(LexList& lex)
{
	json n = node("return");

	n["value"] = parseArgs(lex);

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

		stringstream a;
		lex.stepUp();
		lex.skipSpace();
		while (!gotName)
		{
			a << n;
			if (lex.getType() == "SPACE" || lex.getVal() == ";" || lex.getVal() == ",")
			{
				gotName = 1;
			}
			else
			{
				n["name"] = lex.getVal();
			}
			lex.stepUp();
		}
		if (lex.getVal() == ";")
		{
			break;
		}

		if (!gotArgs && gotName)
		{
			n["children"] = parseArgs(lex);
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
	string val = "\"";
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
	n["value"] = val + "\"";
	return n;
}


json parseArgs(LexList& lex, string sep, string end)
{
	json n =  node("arg");

	while (lex.canRetrieve() && lex.getVal() != end)
	{
		lex.skipSpace();

		if (!lex.canRetrieve() && lex.getType() == "EOF")
		{
			break;
		}
		if (lex.getVal() == end)
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
			continue;
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

	json n = node("int");
	if (dotted)
	{
		n[NN] = "float";
		n["value"] = string(num,1);
	}
	else
	{
		n["value"] = string(num,1);
		
	}

	return n;
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

	bool gotName = 0, gotArgs = 0, gotType = 0, gettingType = 0;
	string name = "", type; // name of function

	while (lex.canRetrieve() && !lex.eof())
	{
		lex.skipSpace();
		if (lex.getVal() == "@")
		{
			gotName = true;
			gettingType = 1;
			lex.stepUp();
			continue;
		}

		if (!gotName)
		{
			name += lex.getVal();
		}

		if (gettingType && !gotType && lex.getType() == "ID")
		{
			type += lex.getVal();
			gotType = 1;
			lex.stepUp();
			continue;
		}
		if (gotArgs)
		{
			n["children"] += parse(lex,name,"}");
			break;
		}
		else if (gotType && gotName)
		{
			gotArgs = true;
			n["args"] = parseArgs(lex, ",", "{");
		}
		lex.stepUp();
	}
	n["type"] = type;
	return n;
}
