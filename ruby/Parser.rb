class Parser

  attr_accessor :tokens, :token_queue, :root
  
  def initialize(tokens=[])
    @tokens = tokens.dup
    @token_queue = tokens.dup
    @root = Token.new(scope: Scope::PARSING, type: Type::PROG)
  end

  def run(tokens, debug: false)
    initialize(tokens)
    @root.children = prog_s
    print_tree if debug
  end

  def prog_s(list=[])
    token = @token_queue.shift
    node = nil

    case token.type
    when Type::BIND
      node = bind_s(token)

    when *Type::DATATYPES
      node = fdef_decls_s(token)

    when Type::FINISH_PARSING
      return list
      
    else
      node = syntax_error("program", token)
    end

    list << node if node != nil

    prog_s(list)
  end

  def fdef_decls_s(token=nil)
    token = @token_queue.shift if token == nil

    case token.type
    when *Type::DATATYPES
      datatype = token.type
      line = token.line
      token = @token_queue.shift

      if token.type == Type::ID
        value = token.value
        token = @token_queue.shift

        # fdef with pars
        if token.type == Type::LPAR
          args = args_s || []

          Token.new(scope: Scope::PARSING, type: Type::FDEF, datatype: datatype, value: value, line: line, children: args)

        # fdef without pars
        elsif token.type == Type::DO
          block = block_s || []

          Token.new(scope: Scope::PARSING, type: Type::FDEF, datatype: datatype, value: value, line: line, children: block)
          
        # decl
        else
          @token_queue.unshift(token)
          Token.new(scope: Scope::PARSING, type: Type::DECL, datatype: datatype, value: value, line: line)
        end

      else
        syntax_error("declaration or function defenition", token)
      end

    else
      syntax_error("declaration or function defenition", token)
    end
  end

  def args_s(args=[])
    token = @token_queue.shift

    case token.type
    # Arg
    when *Type::DATATYPES
      datatype = token.type
      line = token.line
      token = @token_queue.shift

      if token.type == Type::ID
        args << Token.new(scope: Scope::PARSING, type: Type::ARG, datatype: datatype, value: token.value, line: line)
        args_s(args)

      else
        syntax_error("argument list", token)
      end

    else
      return args
    end
  end

  def bind_s(bind_token)
    token = @token_queue.shift

    case token.type
    when Type::STRING_LIT
      Token.new(scope: Scope::PARSING, type: Type::BIND, datatype: Type::STRING, value: token.value, line: bind_token.line)

    else
      syntax_error("bind", token)
    end
  end

  def syntax_error(compiler_source, token)
    puts "Invalid #{compiler_source} at line #{token.line}, #{token.type}"
    return nil
  end

  def print_tree
    puts "\n--- SYNTAX TREE ---\n"
    print_node(@root)
  end

  def print_node(node, identation = 0)
    identation.times { print ".." }
    puts node
  
    node.children.each do |child|
      print_node(child, identation + 1)
    end
  end
end