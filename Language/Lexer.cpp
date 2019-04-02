#include <iostream>

#include <vector>
#include <map>
#include <string>

#include "Lexer.h"
using namespace std;

Type::Type(string a, string b, bool c)
	:name(a), values(b), stackable(c) {}

string Type::getName()
{
	return name;
}

bool Type::isOfType(string a)
{
	return values.find(a) != string::npos;
};

bool Type::isStackable()
{
	return stackable;
}

const int typeSize = 12;

Type types[typeSize] = {
	Type("EQUAL", "="),
	Type("ID", "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz", 1),
	Type("STRSEP", "\"`'"),
	Type("NUM", "1234567890", 1),
	Type("WHITESPACE", " \n", 1),
	Type("QMARK", "?"),
	Type("EXPOINT", "!"),
	Type("PARENTH", "()"),
	Type("CURLY_PAREN", "{}"),
	Type("BRACKET", "[]"),
	Type("OPERATOR", "/*+-"),
	Type("DOT", ".")
};

const int statementsLen = 5;

string statements[statementsLen] = {
	"func",
	"expr",
	"call",
	"var",
	"if"
};

Type getCharType(string ch)
{
	for (int i = 0; i < typeSize; i++)
	{
		if (types[i].isOfType(ch))
		{
			return types[i];
		}
	}
	return Type("NULL", "^");
}

void LexList::add(string type, string val)
{
	types.push_back(type);
	vals.push_back(val);
	length++;
}

string LexList::getType()
{
	return types[index];
}

string LexList::getVal()
{
	return vals[index];
}

void LexList::stepUp(int steps)
{
	index += steps;
}

void LexList::stepDown(int steps)
{
	index -= steps;
}

void LexList::printOut()
{
	for (int i = 0; i < length; i++)
	{
		cout << vals[i] << ": " << types[i] << endl;
	}
}

LexList lex(string text, int linenum)
{
	int index = 0;

	LexList lexed;

	string lastType = "NULL";
	string lastVal = "NULL";
	bool first = 0;

	int len = text.length();

	bool done = 0;

	for (int i = 0; i < len; i++)
	{
		char c = text[i];
		Type theType = getCharType(string(1, c));
		string type = theType.getName();

		if (type == lastType && theType.isStackable())
		{
			lastVal += string(1, c);
		}
		else
		{
			if (first)
			{

				for (int i = 0; i < statementsLen; i++)
				{
					if (statements[i] == lastVal)
					{
						lastType = "STATEMENT";
					}
				}

				lexed.add(lastType, lastVal);
			}
			else
			{
				first = 1;
			}

			    lastType = type;
			lastVal = string(1, c);
		}
	}

	// last part of loop does not get pushed so push it here.
	for (int i = 0; i < statementsLen; i++)
	{
		if (statements[i] == lastVal)
		{
			lastType = "STATEMENT";
		}
	}

	lexed.add(lastType, lastVal);

	lexed.add("EOF", "EOF"); // end of file so AST knows when to stop.

	return lexed;
}