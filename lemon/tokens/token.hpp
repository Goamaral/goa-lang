#ifndef TOKEN_H
#define TOKEN_H

#include <list>
#include <string>
#include <iostream>

using namespace std;

enum class type_t {
  /* DATATYPES */
  VOID, I32, STRING,

  /* OPERATORS */
  LT,

  /* SYNTAX */
  END, LPAR, RPAR, DO, ID, COMMA, SEMI, RBRACE, LBRACE, RETURN, INT_LIT, DOT,
  BIND, IF, ELSE, STRING_LIT,

  /* FINISH_PARSING and EMPTY */
  FINISH_PARSING, EMPTY
};

enum class scope_t {
  NONE,
  LEXER,
  PARSER
};

struct token_t {
  scope_t scope = scope_t::NONE;
  type_t type = type_t::EMPTY;
  string value;
  int line = 0;
  int id;
  list<token_t> children;

  token_t() {}
  token_t(scope_t _scope, type_t _type, list<token_t> _children): scope(_scope), type(_type), children(_children) {}
  token_t(scope_t _scope, type_t _type, string _value, int _line): scope(_scope), type(_type), value(_value), line(_line) {}
  token_t(scope_t _scope, type_t _type, string _value, int _line, int _id): scope(_scope), type(_type), value(_value), line(_line), id(_id) {}

  string to_string();
  string type_to_string();
  string scope_to_string();
  bool is_datatype(type_t);
  bool is_operator(type_t);
};

#endif