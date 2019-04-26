#pragma once

#include <iostream>
#include "Lexer.h"
#include "json.hpp"


nlohmann::json parse(LexList* lex, std::string name = "program");

nlohmann::json parseCall(LexList* lex);

nlohmann::json parseArgs(LexList* lex, std::string sep = ",", std::string end = ";");

nlohmann::json parseString(LexList* lex);

nlohmann::json parseNum(LexList* lex);

nlohmann::json parseIf(LexList* lex);

nlohmann::json node(std::string name);