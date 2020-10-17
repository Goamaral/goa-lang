grammar Calc;

// Tokens
MUL: '*';
DIV: '/';
ADD: '+';
SUB: '-';
NUMBER: [0-9]+;
WHITESPACE: [ \r\n\t]+ -> skip;

// Rules
start : expression EOF;

expression
  : expression op=('*'|'/') expression # BinaryOperation
  | expression op=('+'|'-') expression # BinaryOperation
  | NUMBER                             # Number
  ;