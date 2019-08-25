#include "compiler_builder.hpp"

bool file_found = false;
bool debug_lexer = false;
bool debug_syntax = false;
bool debug_sematics = false;
bool debug_code_gen = false;

string read_file(string file_name) {
  ifstream file(file_name);
  if (file.is_open()) {
    stringstream file_buffer;

    file_buffer << file.rdbuf();
    string code(file_buffer.str());

    file.close();
    file_found = true;

    return code;
  } else {
    cout << "File " << file_name << " not found" << endl;
    return string("");
  }
}

int main(int argc, char** argv) {
  if (argc < 2) {
    cout << "No input files" << endl;
    return 1;
  } else {
    for (int i = 0; i < argc; ++i) {
      if (!strcmp(argv[i], "--debug-lexer")) {
        debug_lexer = true;
      } else if (!strcmp(argv[i], "--debug-syntax")) {
        debug_syntax = true;
      } else if (!strcmp(argv[i], "--debug-sematics")) {
        debug_sematics = true;
      } else if (!strcmp(argv[i], "--debug-code-gen")) {
        debug_code_gen = true;
      } else if (!strcmp(argv[i], "--debug-all")) {
        debug_lexer = true;
        debug_syntax = true;
        debug_sematics = true;
        debug_code_gen = true;
      }
    }
  }

  string code = read_file(argv[1]);

  if (file_found) {
    list<token_t> tokens = lexical(code);
    if (debug_lexer) print_tokens(tokens);

    tree_t syntax_tree = syntax();
    if (debug_syntax) print_tree(syntax_tree);

    string generated_code = generate_code(syntax_tree);
    if (debug_code_gen) cout << endl << generated_code << endl;

    ofstream file("tmp.cpp");
    if (file.is_open()) {
      file << generated_code;
      file.close();

      system("g++ tmp.cpp -o out");
      if (!debug_code_gen) system("rm tmp.cpp");
    }
  }

  return 0;
}