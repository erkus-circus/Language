#include <iostream>
#include <vector>
#include "AST.h"
#include "Lexer.h"

using namespace std;

void Node::printOut()
{
	print();
	for (int i = 0; i < children.size(); i++)
	{
		children[i]->printOut();
	}
}

Node::~Node(){}

void ArgNode::print(){}
void ExecNode::print(){}
void CallNode::print()
{
	cout << "NAME: " << name << endl;
	args.printOut();
	body.printOut();
}
void StringNode::print()
{
	cout << "STRING: " << val << endl;
}

CallNode* parse(LexList* lex, string name)
{

	CallNode* node{};

	while (lex->getType() != "EOF") //reader is not at End of FILE
	{
		lex->stepUp();
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
				node->children.push_back(parseCall(lex));
			}
		}
	}
	return node;
}

CallNode* parseCall(LexList* lex)
{
	CallNode* node{};
	lex->stepDown();

	bool gotName = 0;
	bool gotArgs = 0;
	while (lex->canRetrieve())
	{
		lex->stepUp();
		lex->skipSpace();
		if (!gotName)
		{
			while (!gotName)
			{
				if (lex->getType() == "SPACE")
				{
					gotName = 1;
				}
				else
				{
					node->name += lex->getVal();
				}
			}
		}

		if (!gotArgs)
		{
			node->children.push_back(parseArgs(lex));
		}
	}
	return node;
}


StringNode *parseString(LexList* lex)
{
	StringNode* node{};
	bool esc = 0;
	while (lex->canRetrieve())
	{
		lex->stepUp();
		if (!lex->canRetrieve() && lex->getType() == "EOF")
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
			node->val += lex->getVal();
			continue;
		}
		if (lex->getType() == "STRSEP")
		{
			break;
		}
		node->val += lex->getVal();
	}
	cout << node->val;
	return node;
}


ArgNode* parseArgs(LexList* lex, char sep, char end)
{
	ArgNode* node{};
	while (lex->canRetrieve())
	{
		lex->stepUp();
		if (!lex->canRetrieve() && lex->getType() == "EOF")
		{
			break;
		}
		if (lex->getType() == "STRSEP")
		{
			lex->stepDown();
			node->children.push_back(parseString(lex));
		}
	}
	return node;
}