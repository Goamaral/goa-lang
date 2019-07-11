#include "lexical.hpp"

list<token_t> tokens;
static string token_string = "";
static string::iterator token_it;
static string::iterator source_it;
static int line = 1;

string token_type_string(token_type_t type) {
  switch (type) {
    case VOID: return "VOID";
    case INT: return "INT";
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
  return type == INT || type == VOID;
}


void lock_token(token_type_t type) {
  if (token_string.size() == 0) return;
  tokens.push_back(token_t(type, token_string, line));
  token_string = "";
}

char next_char() {
  if (token_it == token_string.end()) return -1;
  char c = *token_it;
  ++token_it;

  return c;
}

void void_id_s(char prev, token_type_t guess_type = ID) {
  char c = next_char();

  switch (c) {
    case -1:
      return lock_token(guess_type);

    default:
      switch (prev) {
        case 'v':
        case 'V':
          if (c == 'o' || c == 'O') return void_id_s(c);
          return lock_token(ID);
        case 'o':
        case 'O':
          if (c == 'i' || c == 'I') return void_id_s(c);
          return lock_token(ID);
        case 'i':
        case 'I':
          if (c == 'd' || c == 'D') return void_id_s(c, VOID);
          return lock_token(ID);

        default:
          return lock_token(ID);
      }
  }
}

void int_id_s(char prev, token_type_t guess_type = ID) {
  char c = next_char();

  switch (c) {
    case -1:
      return lock_token(guess_type);

    default:
      switch (prev) {
        case 'i':
        case 'I':
          if (c == 'n' || c == 'N') return int_id_s(c);
          return lock_token(ID);
        case 'n':
        case 'N':
          if (c == 't' || c == 'T') return int_id_s(c, INT);
          return lock_token(ID);

        default:
          return lock_token(ID);
      }
  }
}

void end_id_s(char prev, token_type_t guess_type = ID) {
  char c = next_char();

  switch (c) {
    case -1:
      return lock_token(guess_type);

    default:
      switch (prev) {
        case 'e':
        case 'E':
          if (c == 'n' || c == 'N') return end_id_s(c);
          return lock_token(ID);
        case 'n':
        case 'N':
          if (c == 'd' || c == 'D') return end_id_s(c, END);
          return lock_token(ID);

        default:
          return lock_token(ID);
      }
  }
}

void do_id_s(char prev, token_type_t guess_type = ID) {
  char c = next_char();

  switch (c) {
    case -1:
      return lock_token(guess_type);

    default:
      switch (prev) {
        case 'd':
        case 'D':
          if (c == 'o' || c == 'O') return do_id_s(c, DO);
          return lock_token(ID);

        default:
          return lock_token(ID);
      }
  }
}

void return_id_s(char prev, token_type_t guess_type = ID) {
  char c = next_char();

  switch (c) {
    case -1:
      return lock_token(guess_type);

    default:
      switch (prev) {
        case 'r':
        case 'R':
          if (c == 'e' || c == 'E') return return_id_s(c);
          if (c == 'n' || c == 'N') return return_id_s(c, RETURN);
          return lock_token(ID);

        case 'e':
        case 'E':
          if (c == 't' || c == 'T') return return_id_s(c);
          return lock_token(ID);

        case 't':
        case 'T':
          if (c == 'u' || c == 'U') return return_id_s(c);
          return lock_token(ID);

        case 'u':
        case 'U':
          if (c == 'r' || c == 'R') return return_id_s(c);
          return lock_token(ID);

        default:
          return lock_token(ID);
      }
  }
}

void label_token() {
  char c = next_char();

  // int_lit
  regex int_lit_regex("[0-9]+");
  if (regex_match(token_string, int_lit_regex)) {
    lock_token(INT_LIT);
  } else {
    switch (c) {
      // void, id
      case 'v':
      case 'V':
        return void_id_s(c);

      // int, id
      case 'i':
      case 'I':
        return int_id_s(c);

      // end, id
      case 'e':
      case 'E':
        return end_id_s(c);

      // do, id
      case 'd':
      case 'D':
        return do_id_s(c);

      // return, id
      case 'r':
      case 'R':
        return return_id_s(c);

      default:
        return lock_token(ID);
    }
  }
}

void id_locker(token_type_t type) {
  lock_token(ID);
  token_string.push_back(*source_it);
  lock_token(type);
}

list<token_t> lexical(string source) {
  source_it = source.begin();

  while (source_it != source.end()) {
    switch (*source_it) {
      /* spaces */
      case '\n':
      case ' ':
      case '\t':
        if (token_string.size() != 0) {
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
