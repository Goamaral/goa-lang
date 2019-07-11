#ifndef SYNTAX_H
#define SYNTAX_H

#include <list>
#include <string>
#include <iostream>

#include "lexical.hpp"

using namespace std;

enum node_type_t {
  PROG,
  FDEF, DECL, FBDY, ARGS, RETURN_N,
  EMPTY
};

string node_type_string(node_type_t type);

struct tree_t {
  node_type_t type = EMPTY;
  token_type_t datatype = $;
  string value;
  list<tree_t> children;

  tree_t() {};
  tree_t(node_type_t _type): type(_type) {}
};

void print_tree(tree_t);
token_t next_token();

tree_t def_liststat_s();
tree_t syntax();

#endif