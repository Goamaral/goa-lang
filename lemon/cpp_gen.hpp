#ifndef CPP_GEN_H
#define CPP_GEN_H

#include <string>
#include <sstream>
#include <algorithm>
#include <fstream>

#include "tokens/token.hpp"

using namespace std;

string generate_code(token_t);
void Core(int identation, token_t call_node);

#endif