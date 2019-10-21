#include "parser.hpp"

void Parser::print_tokens() {
  cout << endl << "--- TOKENS ---" << endl;

  for (auto token : tokens) {
    cout << token.to_string() << endl;
  }
}

token_t Parser::run() {
  return token_t();
}

void Parser::print_tree(token_t node) {
  cout << endl << "--- SYNTAX TREE ---" << endl;
  Parser::print_node(node, 0);
}

void Parser::print_node(token_t node, int identation) {
  for (int i = 0; i < identation; ++i) {
    cout << "..";
  }

  cout << node.type_to_string();
  cout << "(" << node.value << ")" << endl;

  for (auto child : node.children) {
    Parser::print_node(child, identation + 1);
  }
}