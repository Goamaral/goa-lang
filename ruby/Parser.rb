class Parser

  attr_accessor :tokens, :root
  
  def initialize
    @tokens = []
    @root = Token.new(scope: Scope::PARSER, type: Type::PROG)
  end

  def run(tokens, debug: false)
    initialize
    @tokens = tokens
  end

  def print_tree
    puts "\n--- SYNTAX TREE ---\n"
    print_node(node)

    identation.times { puts ".." }
  end

  def print_node(node, identation = 0)
    puts node.type.to_s
    puts "(#{node.value})"
  
    node.children.each do |child|
      print_node(child, identation + 1)
    end
  end
end