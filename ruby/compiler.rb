require './Lexer.rb'
require './Parser.rb'

def compile(code, debug: false)
  File.write("tmp.cpp", code)
  system("g++ tmp.cpp -o out")
  system("rm tmp.cpp") unless debug 
end

def main
  debug_lexer = false
  debug_syntax = false
  debug_sematics = false
  debug_code_gen = false

  if ARGV.size < 1 
    puts "No input file"
    return 1
  else
    ARGV.each do |arg|
      if (arg == "--debug-lexer")
        debug_lexer = true
      elsif (arg == "--debug-syntax")
        debug_syntax = true
      elsif (arg == "--debug-sematics")
        debug_sematics = true
      elsif (arg == "--debug-code-gen")
        debug_code_gen = true
      elsif (arg == "--debug-all")
        debug_lexer = true
        debug_syntax = true
        debug_sematics = true
        debug_code_gen = true
      end
    end
  end

  file_name = ARGV[0]

  if File.exists?(file_name)
    code = File.read(file_name)

    lexer = Lexer.new
    tokens = lexer.run(code, debug: debug_lexer)

    parser = Parser.new
    syntax_tree = parser.run(tokens, debug: debug_syntax)

    # generator = CodeGenerator.new
    # generated_code = generator.run(syntax_tree, debug: debug_code_gen)

    # compile(generated_code, debug: debug_code_gen)
  else
    puts "#{file_name} not found"
  end

  return 0
end

main()