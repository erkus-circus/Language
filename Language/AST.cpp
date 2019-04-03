#include <iostream>
#include "AST.h"
#include "Lexer.h"

using namespace std;


Node::Node(string name, string val)
	:name(name), val(val) {}

void Node::add(Node node)
{
	this->children.push_back(node);
}

void Node::printOut(){} // TODO
Node parse(LexList lex, string name)
{
	Node node(name);

	lex.stepDown();

	while (lex.getType() != "EOF") // while reader is not at End of FILE
	{
		lex.stepUp();
		if (lex.getType() == "STATEMENT")
		{
			// statement vals
		}
	}
}