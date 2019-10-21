#ifndef PARSER_H
#define PARSER_H

#include "tokens/token.hpp"

#include <list>
#include <iostream>

using namespace std;

struct Parser {
  list<token_t> tokens;

  Parser(list<token_t> _tokens): tokens(_tokens) {}

  void print_tokens();
  token_t run();
  void print_tree(token_t);
  void print_node(token_t, int);
};

#endif