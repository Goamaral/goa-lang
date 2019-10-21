# Grammar
prog -> (bind | fdef | decl) prog
prog -> FINISH_PARSING

bind -> BIND STRING_LIT

fdef -> datatype ID LPAR args RPAR block

decl ->

datatype -> VOID, I32, STRING