class Type
  
  # DATATYPES #
  VOID = :VOID
  I32 = :I32
  STRING = :STRING

  # OPERATORS #
  LT = :LT

  # TOKENS #
  END_ = :END
  LPAR = :LPAR
  RPAR = :RPAR
  DO = :DO
  ID = :ID
  COMMA = :COMMA
  SEMI = :SEMI
  RBRACE = :RBRACE
  LBRACE = :LBRACE
  RETURN = :RETURN
  DOT = :DOT
  BIND = :BIND
  IF = :IF
  ELSE = :ELSE

  # LITERALS #
  INT_LIT = :INT_LIT
  STRING_LIT = :STRING_LIT

  # NODES #
  PROG = :PROG

  # FINISH_PARSING and EMPTY #
  FINISH_PARSING = :FINISH_PARSING
  EMPTY = :EMPTY

  def is_datatype
    [:VOID, :I32, :STRING].include?(self)
  end

end