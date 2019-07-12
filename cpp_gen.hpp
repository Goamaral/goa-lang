#ifndef CPP_GEN_H
#define CPP_GEN_H

#include <string>
#include <sstream>
#include <algorithm>
#include <fstream>

#include "syntax.hpp"
#include "lexical.hpp"

using namespace std;

string generate_code(tree_t);
void Core(int identation, tree_t call_node);

#endif