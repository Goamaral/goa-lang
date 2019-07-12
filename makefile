make:
	g++ compiler_builder.cpp lexical.cpp syntax.cpp cpp_gen.cpp -o compiler

main:
	make && ./compiler code_examples/compiler_builder.goa