#pragma once

#include <iostream>
#include <vector>
#include "Lexer.h"

class Node
{
public:
	Node(std::string name, std::string val = "");
	void add(Node);//comment
	void printOut();

private:
	std::vector<Node> children;
	std::string name;
	std::string val;
};

Node parse(std::string str);
