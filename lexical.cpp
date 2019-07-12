#include "lexical.hpp"

list<token_t> tokens;
static string token_string = "";
static string::iterator token_it;
static string::iterator source_it;
static int line = 1;

string token_type_string(token_type_t type) {
  switch (type) {
    case VOID: return "VOID";
    case I32: return "I32";
    case END: return "END";
    case LPAR: return "LPAR";
    case RPAR: return "RPAR";
    case ID: return "ID";
    case DO: return "DO";
    case COMMA: return "COMMA";
    case SEMI: return "SEMI";
    case RBRACE: return "RBRACE";
    case LBRACE: return "LBRACE";
    case RETURN: return "RETURN";
    case INT_LIT: return "INT_LIT";
    case DOT: return "DOT";
    case BIND: return "BIND";
    case IF: return "IF";
    case LT: return "LT";
    case STRING: return "STRING";
    case $: return "$";
  }

  return "UNKNOWN";
}

void print_tokens(list<token_t> tokens) {
  cout << endl << "--- TOKENS ---" << endl;
  for (auto token : tokens) {
    cout << token_type_string(token.type) << "(" << token.line  << "): " << token.value << endl;
  }
}

bool is_datatype(token_type_t type) {
  return type == I32 || type == VOID;
}

bool is_operator(token_type_t type) {
  return type == LT;
}


void lock_token(token_type_t type) {
  if (token_string.size() == 0) return;
  tokens.push_back(token_t(type, token_string, line));
  token_string = "";
}

void label_token() {
  if (regex_match(token_string, regex("[0-9]+"))) {
    lock_token(INT_LIT);
  } else if (regex_match(token_string, regex("void"))) {
    lock_token(VOID);
  } else if (regex_match(token_string, regex("i32"))) {
    lock_token(I32);
  } else if (regex_match(token_string, regex("end"))) {
    lock_token(END);
  } else if (regex_match(token_string, regex("do"))) {
    lock_token(DO);
  } else if (regex_match(token_string, regex("return"))) {
    lock_token(RETURN);
  } else if (regex_match(token_string, regex("bind"))) {
    lock_token(BIND);
  } else if (regex_match(token_string, regex("if"))) {
    lock_token(IF);
  } else if (regex_match(token_string, regex("<"))) {
    lock_token(LT);
  } else if (regex_match(token_string, regex("\"[^\"]*\""))) {
    lock_token(STRING);
  } else {
    lock_token(ID);
  }
}

void id_locker(token_type_t type) {
  lock_token(ID);
  token_string.push_back(*source_it);
  lock_token(type);
}

list<token_t> lexical(string source) {
  bool capturing_string = false;
  source_it = source.begin();

  while (source_it != source.end()) {
    switch (*source_it) {
      case '"':
        token_string.push_back(*source_it);
        if (capturing_string) lock_token(STRING);
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
        break;

      default:
        switch (*source_it) {
          case '(':
            id_locker(LPAR);
            break;

          case ')':
            id_locker(RPAR);
            break;

          case ',':
            id_locker(COMMA);
            break;

          case ';':
            id_locker(SEMI);
            break;

          case '{':
            id_locker(LBRACE);
            break;

          case '}':
            id_locker(RBRACE);
            break;

          case '.':
            id_locker(DOT);
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

  return tokens;
}
