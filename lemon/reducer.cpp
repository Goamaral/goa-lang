#include "reducer.hpp"

token_t Reducer::bind(token_t t_bind, token_t t_string) {
  return token_t(scope_t::PARSER, type_t::BIND, t_string.value, t_bind.line);
}

token_t Reducer::prog(list<token_t> t_list) {
  return token_t(scope_t::PARSER, type_t::PROG, t_list);
}