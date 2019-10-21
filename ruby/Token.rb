require './Scope.rb'
require './Type.rb'

class Token

  attr_accessor :scope, :type, :value, :line, :id, :children

  def initialize(args)
    @scope = args[:scope] || Scope::NONE
    @type = args[:type] || Type::EMPTY
    @value = args[:value] || ""
    @line = args[:line] || 0
    @id = args[:id] || -1
    @children = args[:children] || []
  end

  def to_s
    "#{@type.to_s} (id: #{@id}, line: #{@line}): #{@value}"
  end

  def node_to_s
    if @value != ""
      "#{@type.to_s} (n_children: #{@children.size}): #{@value}"
    else
      "#{@type.to_s} (n_children: #{@children.size})"
    end
  end

end