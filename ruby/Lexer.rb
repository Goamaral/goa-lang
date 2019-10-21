require './Token.rb'

class Lexer

  attr_accessor :code, :tokens, :token_string, :current_c

  def initialize
    @code = ""
    @tokens = []
    @token_string = ""
    @current_c = ""
    @line = 1
  end

  def run(code, debug: false)
    initialize
    @code = code
    capturing_string = false

    @code.split('').each do |c|
      @current_c = c

      case c
      when "\""
        @token_string << c
        lock_token(Type::STRING_LIT) if capturing_string
        capturing_string = !capturing_string

      else
        if capturing_string
          @token_string << c
        else
          case c
          when "\n", " ", "\t"
            label_token if @token_string.size != 0
            @token_string << c if capturing_string

          when "("
            id_locker(Type::LPAR)

          when ")"
            id_locker(Type::RPAR)

          when ","
            id_locker(Type::COMMA)

          when ";"
            id_locker(Type::SEMI)

          when '{'
            id_locker(Type::LBRACE)

          when '}'
            id_locker(Type::RBRACE)

          when '.'
            id_locker(Type::DOT)

          else
            @token_string << c
          end
        end
      end

      @line += 1 if c == "\n"
    end

    label_token if @token_string.size != 0

    @tokens << Token.new(scope: Scope::LEXING, type: Type::FINISH_PARSING, value: "$", line: @line, id: tokens.size)

    print_tokens if debug

    return @tokens
  end

  def lock_token(type)
    if @token_string.size != 0
      @tokens << Token.new(scope: Scope::LEXING, type: type, value: @token_string, line: @line, id: tokens.size)
      @token_string = ""
    end
  end

  def label_token
    case @token_string
    when /void/
      lock_token(Type::VOID)

    when /i32/
      lock_token(Type::I32)

    when /string/
      lock_token(Type::STRING)

    when /[0-9]+/
      lock_token(Type::INT_LIT)

    when /end/
      lock_token(Type::END_)

    when /do/
      lock_token(Type::DO)

    when /return/
      lock_token(Type::RETURN)

    when /bind/
      lock_token(Type::BIND)

    when /if/
      lock_token(Type::IF)

    when /else/
      lock_token(Type::ELSE)

    when /</
      lock_token(Type::LT)

    when /\"[^\"]*\"/
      lock_token(Type::STRING_LIT)

    else
      lock_token(Type::ID)
    end
  end

  def id_locker(type)
    lock_token(Type::ID)
    @token_string << @current_c
    lock_token(type)
  end

  def print_tokens
    puts "\n--- TOKENS ---\n"
    @tokens.each do |token|
      puts token.to_s
    end
  end

end