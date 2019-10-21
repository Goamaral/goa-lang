#ifndef LEXICAL_H
#define LEXICAL_H

#include <list>
#include <string>
#include <iostream>
#include <regex>
#include "tokens/token.hpp"

using namespace std;

struct Lexer {
  list<token_t> tokens;
  string token_string = "";
  string::iterator token_it;
  string source;
  string::iterator source_it;
  int line = 1;

  Lexer(string _source): source(_source) {}

  list<token_t> run();
  void lock_token(type_t);
  char next_char();
  void id_locker(type_t);
  void label_token();
  void print_tokens();
};

#endif