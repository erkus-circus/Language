#pragma once

#include <iostream>
#include <vector>
#include "Lexer.h"

class Node
{
public:
	std::vector<Node *> children;
	void printOut();
	virtual void print() = 0;
	virtual ~Node();
};

class ArgNode  : public Node {
public:
	void print();
};

class ExecNode : public Node
{
public:
	void print();
};

class CallNode : public Node
{
public:
	void print();
	std::string name = "";
	ArgNode args;
	ExecNode body;
};

class StringNode : public Node
{
public:
	std::string val = "";
	void print();
};

CallNode* parse(LexList* lex, std::string name = "program");

CallNode *parseCall(LexList* lex);

ArgNode *parseArgs(LexList* lex, char sep=',', char end=';');

StringNode* parseString(LexList* lex);