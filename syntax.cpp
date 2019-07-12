#include "syntax.hpp"

extern list<token_t> tokens;

static list<token_t>::iterator tokens_it;
static list<token_t> _stack;
tree_t tree(PROG);
bool syntax_errors = false;

string node_type_string(node_type_t type) {
  switch (type) {
    case PROG: return "PROG";
    case FDEF: return "FDEF";
    case DECL: return "DECL";
    case BODY: return "BODY";
    case EMPTY: return "EMPTY";
    case ARGS: return "ARGS";
    case RETURN_N: return "RETURN";
    case CALL: return "CALL";
    case BIND_N: return "BIND_N";
    case IF_N: return "IF_N";
    case EXPR: return "EXPR";
    case ID_N: return "ID_N";
    case OP_N: return "OP_N";
    case STRING_N: return "STRING_N";
    case METHOD_CALL: return "METHOD_CALL";
  }

  return "UNKNOWN";
}

void print_stack(string id = string()) {
  list<token_t>::reverse_iterator stack_it = _stack.rbegin();

  cout << "--- STACK " << id << " ---" << endl;

  while (stack_it != _stack.rend()) {
    cout << token_type_string(stack_it->type) << " " << stack_it->value << endl;
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
  if (tokens_it != tokens.end()) _stack.push_back(*tokens_it++);
}

void push_stack(token_t token) {
  _stack.push_back(token);
}

token_t pop_stack() {
  token_t token = _stack.back();
  _stack.pop_back();

  return token;
}

token_t peek_stack() {
  return _stack.back();
}

void invalid_syntax(token_t token, string compiler_source) {
  syntax_errors = true;
  destroy_token();
  cout << "Invalid syntax (" << compiler_source << "), line " << token.line << ": " << token_type_string(token.type) << endl;
}

tree_t reduce_fdef(int line, tree_t args, tree_t body) {
  token_t id = pop_stack();
  token_t datatype = pop_stack();

  tree_t node(FDEF);
  node.datatype = datatype.type;
  node.value = id.value;
  node.line = line;
  node.children.push_back(args);
  node.children.push_back(body);

  return node;
}

tree_t reduce_decl(int line) {
  token_t id = pop_stack();
  token_t datatype = pop_stack();

  tree_t node(DECL);
  node.datatype = datatype.type;
  node.value = id.value;
  node.line = line;

  return node;
}

tree_t reduce_return(int line, token_type_t datatype) {
  token_t id = pop_stack();

  tree_t node(RETURN_N);
  node.datatype = datatype;
  node.value = id.value;
  node.line = line;

  return node;
}

tree_t reduce_bind(int line) {
  token_t id = pop_stack();

  tree_t node(BIND_N);
  node.value = id.value;
  node.line = line;

  return node;
}

tree_t reduce_call(int line, list<tree_t> args) {
  token_t id = pop_stack();

  tree_t node(CALL);
  node.value = id.value;
  node.line = line;
  node.children = args;

  return node;
}

tree_t reduce_if(int line, tree_t cond, tree_t body) {
  tree_t node(IF_N);
  node.value = "if";
  node.line = line;
  node.children.push_back(cond);
  node.children.push_back(body);

  return node;
}

tree_t reduce_method_call(int line, tree_t call) {
  token_t id = pop_stack();

  tree_t node(METHOD_CALL);
  node.value = id.value;
  node.line = line;
  node.children.push_back(call);

  return node;
}

tree_t expr_s(tree_t expr = tree_t(EXPR)) {
  token_t token = peek_token();
  int line = token.line;

  if (token.type == ID || token.type == INT_LIT) {
    destroy_token();

    tree_t id(ID_N);
    id.value = token.value;
    id.line = line;
    expr.children.push_back(id);

    token = peek_token();

    if (is_operator(token.type)) {
      destroy_token();

      tree_t op(OP_N);
      op.value = token.value;
      op.datatype = token.type;
      op.line = line;
      expr.children.push_back(op);

      return expr_s(expr);
    } else  {
      return expr;
    }

  } else if (token.type == LPAR) {
    destroy_token();
    tree_t expr = expr_s(expr);

    if (expr.type != EMPTY && token.type == RPAR) {
      tree_t lpar(OP_N);
      lpar.datatype = LPAR;
      lpar.line = line;
      lpar.value = "(";
      expr.children.push_front(lpar);

      tree_t rpar(OP_N);
      lpar.datatype = RPAR;
      lpar.line = line;
      lpar.value = ")";
      expr.children.push_back(rpar);

      return expr;
    }
  }

  invalid_syntax(token, "expr_s");

  return tree_t();
}

tree_t decl_s() {
  token_t token = peek_token();
  int line = token.line;

  if (is_datatype(token.type)) {
    consume_token();
    token = peek_token();

    if (token.type == ID) {
      consume_token();
      return reduce_decl(line);
    }
  }

  invalid_syntax(token, "decl_s");

  return tree_t();
}

tree_t return_s(token_type_t datatype) {
  token_t token = peek_token();
  int line = token.line;

  if (token.type == RETURN) {
    destroy_token();
    token = peek_token();

    if (token.type == ID || token.type == INT_LIT) {
      consume_token();
      return reduce_return(line, datatype);
    }
  }

  invalid_syntax(token, "return_s");

  return tree_t();
}

tree_t call_methodcall_s(bool from_method_call = false) {
  token_t token = peek_token();
  int line = token.line;

  if (token.type == ID) {
    consume_token();
    token = peek_token();

    // Call
    if (token.type == LPAR) {
      destroy_token();
      token = peek_token();
      list<tree_t> args;

      while (token.type == ID || token.type == STRING) {
        tree_t arg;
        if (token.type == ID) {
          arg = expr_s();
        } else if (token.type == STRING) {
          destroy_token();

          arg = tree_t(STRING_N);
          arg.line = token.line;
          arg.datatype = token.type;
          arg.value = token.value;
        }

        args.push_back(arg);
        token = peek_token();

        if (token.type == COMMA) {
          destroy_token();
          token = peek_token();
        } else if (token.type != RPAR) {
          break;
        }
      }

      if (token.type == RPAR) {
        destroy_token();
        return reduce_call(line, args);
      }
    }
    // Method Call
    else if (token.type == DOT && !from_method_call) {
      destroy_token();
      token = peek_token();

      tree_t call = call_methodcall_s(true);
      return reduce_method_call(line, call);
    }
  }

  invalid_syntax(token, "call_s");

  return tree_t();
}

tree_t if_s(token_type_t datatype) {
  token_t token = peek_token();
  int line = token.line;

  if (token.type == IF) {
    destroy_token();
    tree_t cond = expr_s();
    token = peek_token();

    if (token.type == DO) {
      destroy_token();
      tree_t if_body = body_s(datatype);

      token = peek_token();
      if (token.type == END) {
        destroy_token();
        return reduce_if(line, cond, if_body);
      }
    }
  }

  invalid_syntax(token, "if_s");

  return tree_t();
}

tree_t body_s(token_type_t datatype) {
  token_t token = peek_token();
  tree_t node(BODY);
  node.line = token.line;

  while (token.type != END) {
    // Decl
    if (is_datatype(token.type)) {
      tree_t child = decl_s();
      if (child.type != EMPTY) node.children.push_back(child);

      token = peek_token();

      // Further Dec
      while (token.type == COMMA) {
        destroy_token();
        token = peek_token();

        if (token.type == ID) {
          push_stack(token_t(child.datatype));
          consume_token();

          child = reduce_decl(token.line);
          if (child.type != EMPTY) node.children.push_back(child);
        } else {
          invalid_syntax(token, "body_s");
        }

        token = peek_token();
      }
    }
    // Return
    else if (token.type == RETURN) {
      tree_t child = return_s(datatype);
      if (child.type != EMPTY) node.children.push_back(child);
    }
    // Call, METHOD_CALL
    else if (token.type == ID) {
      tree_t child = call_methodcall_s();
      if (child.type != EMPTY) node.children.push_back(child);
    }
    // IF
    else if (token.type == IF) {
      tree_t child = if_s(datatype);
      if (child.type != EMPTY) node.children.push_back(child);
    } else {
      invalid_syntax(token, "body_s");
    }

    token = peek_token();
  }

  return node;
}

tree_t args_s() {
  token_t token = peek_token();
  tree_t node(ARGS);
  node.line = token.line;

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
  int line = token.line;

  if (is_datatype(token.type)) {
    token_type_t datatype = token.type;
    consume_token();
    token = peek_token();

    if (token.type == ID) {
      int id_line = token.line;
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
            tree_t body = body_s(datatype);
            token = peek_token();

            if (token.type == END) {
              destroy_token();
              return reduce_fdef(line, args, body);
            }
          }
        }
      }
      // fdef without pars
      else if (token.type == DO) {
        destroy_token();
        tree_t body = body_s(datatype);
        token = peek_token();

        if (token.type == END) {
          destroy_token();
          tree_t args(ARGS);
          args.line = id_line;
          return reduce_fdef(line, args, body);
        }
      }
      // dec
      else {
        return reduce_decl(token.line);
      }
    }
  }

  invalid_syntax(token, "fdef_decls_s");

  return tree_t();
}

tree_t bind_s() {
  token_t token = peek_token();
  int line = token.line;

  if (token.type == BIND) {
    destroy_token();
    token = peek_token();

    if (token.type == ID) {
      consume_token();
      return reduce_bind(line);
    }
  }

  invalid_syntax(token, "bind_s");

  return tree_t();
}

void print_node(tree_t node, int identation = 0) {
  for (int i = 0; i < identation; ++i) {
    cout << "..";
  }

  cout << node_type_string(node.type);

  if (node.type == DECL || node.type == FDEF || node.type == RETURN_N) {
    cout << "(" << token_type_string(node.datatype) << ", " << node.value << ")";
  } else if (node.type == CALL || node.type == BIND_N || node.type == METHOD_CALL || node.type == ID_N || node.type == OP_N || node.type == STRING_N) {
    cout << "(" << node.value << ")";
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
  tree_t child;

  while (token.type != $) {
    switch (token.type) {
      case BIND:
        child = bind_s();
        break;

      default:
        if (is_datatype(token.type)) {
          child = fdef_decls_s();
        } else {
          invalid_syntax(token, "syntax");
        }
        break;
    }

    if (child.type != EMPTY) tree.children.push_back(child);
    token = peek_token();
  }

  if (syntax_errors) {
    return tree_t();
  } else {
    return tree;
  }
}