#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include "json.hpp"

using json = nlohmann::json;

std::string compile(json js);