#pragma once

#include <iostream>
#include "Lexer.h"
#include "json.hpp"

using json = nlohmann::json;

json parse(LexList& lex, std::string name = "program", std::string end = "");

json parseCall(LexList& lex);

json parseArgs(LexList& lex, std::string sep = ",", std::string end = ";");

json parseString(LexList& lex);

json parseNum(LexList& lex);

json parseIf(LexList& lex);

json parseFunction(LexList& lex);

json parseVarDec(LexList& lex); // parse variable declaration

json node(std::string name);