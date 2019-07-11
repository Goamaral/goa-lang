#include "syntax.hpp"

extern list<token_t> tokens;

static list<token_t>::iterator tokens_it;
static list<token_t> stack;
tree_t tree(PROG);
bool syntax_errors = false;

string node_type_string(node_type_t type) {
  switch (type) {
    case PROG: return "PROG";
    case FDEF: return "FDEF";
    case DECL: return "DECL";
    case FBDY: return "FBDY";
    case EMPTY: return "EMPTY";
    case ARGS: return "ARGS";
  }

  return "UNKNOWN";
}

void print_stack(string id = string()) {
  list<token_t>::reverse_iterator stack_it = stack.rbegin();

  cout << "--- STACK " << id << " ---" << endl;

  while (stack_it != stack.rend()) {
    cout << token_type_string(stack_it->type) << endl;
    ++stack_it;
  }

  cout << "-----------" << endl;
}

token_t peek_token() {
  if (tokens_it == tokens.end()) return token_t($);
  return *tokens_it;
}

void destroy_token() {
  tokens_it++;
}

void consume_token() {
  if (tokens_it != tokens.end()) stack.push_back(*tokens_it++);
}

void push_stack(token_t token) {
  stack.push_back(token);
}

token_t pop_stack() {
  token_t token = stack.back();
  stack.pop_back();

  return token;
}

void invalid_syntax(token_t token, string compiler_source) {
  syntax_errors = true;
  destroy_token();
  cout << "Invalid syntax (" << compiler_source << "), line " << token.line << ": " << token_type_string(token.type) << endl;
}

tree_t reduce_fdef(tree_t args, tree_t body, bool with_pars) {
  token_t id = pop_stack();
  token_t datatype = pop_stack();

  tree_t node(FDEF);
  node.datatype = datatype.type;
  node.value = id.value;
  node.children.push_back(args);
  node.children.push_back(body);

  return node;
}

tree_t reduce_decl() {
  token_t id = pop_stack();
  token_t datatype = pop_stack();

  tree_t node(DECL);
  node.datatype = datatype.type;
  node.value = id.value;

  return node;
}

tree_t decl_s() {
  token_t token = peek_token();

  if (is_datatype(token.type)) {
    consume_token();
    token = peek_token();

    if (token.type == ID) {
      consume_token();
      return reduce_decl();
    }
  }

  invalid_syntax(token, "decl_s");

  return tree_t();
}

tree_t fbdy_s() {
  token_t token = peek_token();
  tree_t node(FBDY);

  while (token.type != END) {
    // Dec next
    if (is_datatype(token.type)) {
      tree_t child = decl_s();
      if (child.type != EMPTY) node.children.push_back(child);

      token = peek_token();

      // Further Dec
      while (token.type == COMMA) {
        destroy_token();
        token = peek_token();

        cout << token_type_string(token.type) << endl;

        if (token.type == ID) {
          push_stack(token_t(child.datatype));
          consume_token();

          child = reduce_decl();
          if (child.type != EMPTY) node.children.push_back(child);
        } else {
          invalid_syntax(token, "fbdy_s");
        }

        token = peek_token();
      }
    } else {
      invalid_syntax(token, "fbdy_s");
    }

    token = peek_token();
  }

  return node;
}

tree_t args_s() {
  token_t token = peek_token();
  tree_t node(ARGS);

  while (is_datatype(token.type)) {
    tree_t child = decl_s();

    if (child.type != EMPTY) {
      node.children.push_back(child);
    }

    token = peek_token();
    if (token.type == COMMA) {
      destroy_token();
      token = peek_token();
    }
  }

  if (token.type != RPAR) {
    invalid_syntax(token, "args_s");
    return node;
  }

  return node;
}

tree_t fdef_decls_s() {
  token_t token = peek_token();

  if (is_datatype(token.type)) {
    consume_token();
    token = peek_token();

    if (token.type == ID) {
      consume_token();
      token = peek_token();

      // fdef with pars
      if (token.type == LPAR) {
        destroy_token();
        tree_t args = args_s();
        token = peek_token();

        if (token.type == RPAR) {
          destroy_token();
          token = peek_token();

          if (token.type == DO) {
            destroy_token();
            tree_t body = fbdy_s();
            token = peek_token();

            if (token.type == END) {
              destroy_token();
              return reduce_fdef(args, body, true);
            }
          }
        }
      }
      // fdef without pars
      else if (token.type == DO) {
        destroy_token();
        tree_t body = fbdy_s();
        token = peek_token();

        if (token.type == END) {
          destroy_token();
          return reduce_fdef(tree_t(ARGS), body, false);
        }
      }
      // dec
      else if (token.type == ID) {
        return reduce_decl();
      }
    }
  }

  invalid_syntax(token, "fdef_decls_s");

  return tree_t();
}

void print_node(tree_t node, int identation = 0) {
  for (int i = 0; i < identation; ++i) {
    cout << "..";
  }

  cout << node_type_string(node.type);

  if (node.type == DECL || node.type == FDEF) {
    cout << "(" << token_type_string(node.datatype) << ", " << node.value << ")";
  }

  cout << endl;

  for (auto child : node.children) {
    print_node(child, identation + 1);
  }
}

void print_tree(tree_t node) {
  cout << endl << "--- SYNTAX TREE ---" << endl;
  print_node(node);
}

tree_t syntax() {
  tokens_it = tokens.begin();
  token_t token = peek_token();

  while (token.type != $) {
    tree_t child = fdef_decls_s();

    if (child.type != EMPTY) {
      tree.children.push_back(child);
    }

    token = peek_token();
  }

  if (syntax_errors) {
    return tree_t();
  } else {
    return tree;
  }
}