require './Scope.rb'
require './Type.rb'

class Token

  attr_accessor :scope, :type, :datatype, :value, :line, :id, :children

  def initialize(args)
    @scope = args[:scope] || Scope::NONE
    @type = args[:type] || Type::EMPTY
    @datatype = args[:datatype] || Type::EMPTY
    @value = args[:value] || ""
    @line = args[:line] || 0
    @id = args[:id] || -1
    @children = args[:children] || []
  end

  def to_s
    args = []
    args << "datatype: #{@datatype}" unless @datatype == Type::EMPTY
    args << "value: #{value}" unless @value == ""
    args << "line: #{line}" unless @line == 0
    args << "id: #{id}" unless @id == -1
    args_s = args.size == 0 ? "" : "(#{args.join(", ")})"

    "#{@type} #{args_s}"
  end

end