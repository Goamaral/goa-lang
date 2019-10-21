#include "token.hpp"

void token_t::to_string() {
  cout << type_to_s(type) << "(" << id << ", " << line  << "): " << value << endl;
}

string token_t::type_to_string() {
  switch (type) {
    case type_t::VOID: return "VOID";
    case type_t::I32: return "I32";
    case type_t::END: return "END";
    case type_t::LPAR: return "LPAR";
    case type_t::RPAR: return "RPAR";
    case type_t::ID: return "ID";
    case type_t::DO: return "DO";
    case type_t::COMMA: return "COMMA";
    case type_t::SEMI: return "SEMI";
    case type_t::RBRACE: return "RBRACE";
    case type_t::LBRACE: return "LBRACE";
    case type_t::RETURN: return "RETURN";
    case type_t::INT_LIT: return "INT_LIT";
    case type_t::DOT: return "DOT";
    case type_t::BIND: return "BIND";
    case type_t::IF: return "IF";
    case type_t::LT: return "LT";
    case type_t::STRING: return "STRING";
    case type_t::ELSE: return "ELSE";
    case type_t::$: return "$";
  }

  return "UNKNOWN";
}

bool token_t::is_datatype(type_t type) {
  return type == type_t::I32 || type == type_t::VOID;
}

bool token_t::is_operator(type_t type) {
  return type == type_t::LT;
}
