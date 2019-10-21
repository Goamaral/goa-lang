class TestLanguage
  token FINISH_PARSING BIND STRING_LIT
  start prog

  convert
    STRING_LIT  'String'
  end

  rule
    prog: prog_list { result = { type: "PROG", value: "", children: val[0] } }

  prog_list: bind prog_list { val[1] << val[0]; result = val[1] }
    | FINISH_PARSING { result = [] }

  bind: BIND STRING_LIT { result = { type: "BIND", value: val[1], children: [] } }
end