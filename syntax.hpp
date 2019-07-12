#ifndef SYNTAX_H
#define SYNTAX_H

#include <list>
#include <string>
#include <iostream>

#include "lexical.hpp"

using namespace std;

enum node_type_t {
  PROG, FDEF, DECL, BODY, ARGS, RETURN_N, CALL, ARG, METHOD_CALL, BIND_N, EXPR,
  ID_N, OP_N, IF_N,
  EMPTY
};

struct tree_t {
  node_type_t type = EMPTY;
  token_type_t datatype = $;
  string value;
  list<tree_t> children;
  int line = 0;

  tree_t() {};
  tree_t(node_type_t _type): type(_type) {}
};

void print_tree(tree_t);
token_t next_token();

tree_t def_liststat_s();
tree_t syntax();
string node_type_string(node_type_t type);
tree_t body_s(token_type_t);

#endif