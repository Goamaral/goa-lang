# Grammar
prog -> (bind | fdef | decl) prog
prog -> FINISH_PARSING

bind -> BIND STRING_LIT

fdef -> datatype ID args block
fdef -> datatype ID block

decl -> datatype ID

args -> LPAR arg_list RPAR

block -> DO 

arg_list -> arg next_arg

arg -> datatype ID

next_arg -> COMMA arg next_arg
next_arg -> LAMBDA

datatype -> VOID, I32, STRING

# Nodes

PROG: children
BIND: datatype, value
FDEF: datatype, value, children: { ARGS, BLOCK }
DECL: datatype, value
ARGS: children: [ARG]
BLOCK: children: [DECL]
ARG: datatype, value