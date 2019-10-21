#include "lexer.hpp"

void Lexer::lock_token(type_t type) {
  if (token_string.size() == 0) return;
  tokens.push_back(token_t(scope_t::LEXER, type, token_string, line, tokens.size()));
  token_string = "";
}

void Lexer::label_token() {
  if (regex_match(token_string, regex("[0-9]+"))) {
    lock_token(type_t::INT_LIT);
  } else if (regex_match(token_string, regex("void"))) {
    lock_token(type_t::VOID);
  } else if (regex_match(token_string, regex("i32"))) {
    lock_token(type_t::I32);
  } else if (regex_match(token_string, regex("string"))) {
    lock_token(type_t::STRING);
  } else if (regex_match(token_string, regex("end"))) {
    lock_token(type_t::END);
  } else if (regex_match(token_string, regex("do"))) {
    lock_token(type_t::DO);
  } else if (regex_match(token_string, regex("return"))) {
    lock_token(type_t::RETURN);
  } else if (regex_match(token_string, regex("bind"))) {
    lock_token(type_t::BIND);
  } else if (regex_match(token_string, regex("if"))) {
    lock_token(type_t::IF);
  } else if (regex_match(token_string, regex("else"))) {
    lock_token(type_t::ELSE);
  } else if (regex_match(token_string, regex("<"))) {
    lock_token(type_t::LT);
  } else if (regex_match(token_string, regex("\"[^\"]*\""))) {
    lock_token(type_t::STRING_LIT);
  } else {
    lock_token(type_t::ID);
  }
}

void Lexer::id_locker(type_t type) {
  lock_token(type_t::ID);
  token_string.push_back(*source_it);
  lock_token(type);
}

list<token_t> Lexer::run() {
  bool capturing_string = false;
  source_it = source.begin();

  while (source_it != source.end()) {
    switch (*source_it) {
      case '"':
        token_string.push_back(*source_it);
        if (capturing_string) lock_token(type_t::STRING);
        capturing_string = !capturing_string;
        break;

      /* spaces */
      case '\n':
      case ' ':
      case '\t':
        if (!capturing_string && token_string.size() != 0) {
          token_it = token_string.begin();
          label_token();
        }

        if (capturing_string) token_string.push_back(*source_it);
        break;

      default:
        switch (*source_it) {
          case '(':
            id_locker(type_t::LPAR);
            break;

          case ')':
            id_locker(type_t::RPAR);
            break;

          case ',':
            id_locker(type_t::COMMA);
            break;

          case ';':
            id_locker(type_t::SEMI);
            break;

          case '{':
            id_locker(type_t::LBRACE);
            break;

          case '}':
            id_locker(type_t::RBRACE);
            break;

          case '.':
            id_locker(type_t::DOT);
            break;

          default:
            token_string.push_back(*source_it);
        }
    }

    if (*source_it == '\n') ++line;

    ++source_it;
  }

  if (token_string.size() != 0) {
    token_it = token_string.begin();
    label_token();
  }

  tokens.push_back(token_t(scope_t::LEXER, type_t::FINISH_PARSING, "", line, tokens.size()));

  return tokens;
}


void Lexer::print_tokens() {
  cout << endl << "--- TOKENS ---" << endl;
  for (auto token : tokens) {
    cout << token.type_to_string() << "(" << token.id << ", " << token.line  << "): " << token.value << endl;
  }
}