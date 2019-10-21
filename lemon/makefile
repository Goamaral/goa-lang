bison:
	bison -dv bison_parser.y

compiler:
	g++ compiler_builder.cpp lexer.cpp parser.cpp bison_parser.tab.cc cpp_gen.cpp -o compiler