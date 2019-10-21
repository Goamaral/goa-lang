#include "tokens/token.hpp"

#ifndef TOKEN_H
#define TOKEN_H

struct Reducer {
  token_t bind(token_t, token_t);
  token_t prog(list<token_t>);
}

#endif