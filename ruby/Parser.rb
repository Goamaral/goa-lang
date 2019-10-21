class Parser

  attr_accessor :tokens, :token_stack, :root
  
  def initialize(tokens=[])
    @tokens = tokens.dup
    @token_stack = tokens.dup
    @root = Token.new(scope: Scope::PARSING, type: Type::PROG)
  end

  def run(tokens, debug: false)
    initialize(tokens)
    @root.children = prog_s
    print_tree if debug
  end

  def prog_s(list=[])
    token = @token_stack.shift
    node = nil

    case token.type
    when Type::BIND
      node = bind_s(token)

    when Type.methods(:is_datatype)
      @token_stack.unshift(token)
      node = fdef_decls_s

    when Type::FINISH_PARSING
      return list
      
    else
      node = syntax_error("program", token)
    end

    list << node if node != nil

    prog_s(list)
  end

  def fdef_decls_s
    token = @token_stack.shift

    # HERE    
  end

  def bind_s(bind_token)
    token = @token_stack.shift
    source = nil

    case token.type
    when Type::STRING_LIT
      source = token.value

    else
      return syntax_error("bind", token)
    end

    Token.new(scope: Scope::PARSING, type: Type::BIND, value: source, line: bind_token.line)
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
    puts node.node_to_s
  
    node.children.each do |child|
      print_node(child, identation + 1)
    end
  end
end