make:
	g++ compiler_builder.cpp lexical.cpp syntax.cpp cpp_gen.cpp -o compiler

main:
	./compiler code_examples/main.goa