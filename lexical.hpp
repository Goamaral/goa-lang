#ifndef LEXICAL_H
#define LEXICAL_H

#include <list>
#include <string>
#include <iostream>

using namespace std;

enum token_type_t {
  /* DATATYPES */
  VOID, INT,

  /* OPERATORS */

  /* SYNTAX */
  END, LPAR, RPAR, DO, ID, COMMA, SEMI, RBRACE, LBRACE,

  // $
  $
};

struct token_t {
  token_type_t type = $;
  string value;
  int line;

  token_t() {}
  token_t(token_type_t _type) : type(_type) {}
  token_t(token_type_t _type, string _value, int _line) : type(_type), value(_value), line(_line) {}
};

void print_tokens(list<token_t>);
string token_type_string(token_type_t);
bool is_datatype(token_type_t);

void lock_token(token_type_t);
char next_char();

void void_id_s(char, token_type_t);
void int_id_s(char, token_type_t);
void end_id_s(char, token_type_t);
void do_id_s(char, token_type_t);
void id_locker(token_type_t);
void label_token();

list<token_t> lexical(string);

#endif