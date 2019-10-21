
%include {
  #include <list>

  #include "parser.hpp"
  #include "reducer.hpp"

  using namespace std;
}

%parse_failure {
  cout << "ERROR: Failed to parse" << endl;
}

%syntax_error {
  cout << "ERROR: Syntax error" << endl;
}

%token_prefix LEMON_

%token_type {token_t}
%start_symbol prog

%type prog_list {list<token_t>}

prog(TOKEN) ::= prog_list(T_LIST). { TOKEN = Reducer::prog(T_LIST); }

prog_list(TOKEN) ::= bind(T_BIND) prog_list(T_LIST). { T_LIST.push_front(T_BIND); TOKEN = T_LIST; }
prog_list(TOKEN) ::= FINISH_PARSING. { TOKEN = list<token_t>(); }

bind(TOKEN) ::= BIND(T_BIND) STRING_LIT(T_STRING). { TOKEN = Reducer::bind(T_BIND, T_STRING); }