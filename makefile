all: Calc.g4
	antlr4 -Dlanguage=Go -o parser -Werror Calc.g4