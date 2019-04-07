#pragma once

#include <string>
#include <map>
#include <vector>

class Type {
public:
	Type(std::string a, std::string b, bool c = 0);
	bool isOfType(std::string);
	std::string getName();
	bool isStackable();
private:
	const std::string name;
	const std::string values;
	const bool stackable;
};

class LexList {
public:
	void add(std::string, std::string);
	std::string getType();
	std::string getVal();
	void stepUp(int steps = 1);
	void stepDown(int steps = 1);
	void skipSpace();
	void printOut();
	int getIndex();
	int getLength();
	bool canRetrieve();
private:
	int index = 0;
	int length = 0;
	std::vector<std::string> types;
	std::vector<std::string> vals;
};

LexList lex(std::string, int linenum = 0);
Type getCharType(std::string);