%require "3.4"
%language "c++"

%{
  #include <list>

  #include "parser.hpp"
  #include "reducer.hpp"

  using namespace std;
%}

%type < list<token_t> > prog_list
%type <token_t> prog bind

%token <token_t> FINISH_PARSING BIND STRING_LIT

%%
prog: 
  prog_list { $$ = Reducer::prog($1); }
  ;

prog_list:
  bind prog_list { $2.push_front($1); $$ = $2; }
  | FINISH_PARSING { $$ = $1; }
  ;

bind:
  BIND STRING_LIT { $$ = Reducer::bind($1, $2); }
  ;
%%

int yyerror(char *s) {
  cout << "Parsing error" << endl;
  return 0;
}