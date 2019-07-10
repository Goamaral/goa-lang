#include "compiler_builder.hpp"

bool file_not_found = false;

string read_file(string file_name) {
  ifstream file(file_name);
  if (file.is_open()) {
    stringstream file_buffer;

    file_buffer << file.rdbuf();
    string code(file_buffer.str());

    file.close();

    return code;
  } else {
    file_not_found = true;
    cout << "File " << file_name << " not found" << endl;
    return string("");
  }
}

int main(int argc, char** argv) {
  if (argc < 2) {
    cout << "No input files" << endl;
    return 1;
  }

  string code = read_file(argv[1]);

  if (!file_not_found) {
    list<token_t> tokens = lexical(code);
    print_tokens(tokens);

    tree_t syntax_tree = syntax();
    print_tree(syntax_tree);

    string generated_code = generate_code(syntax_tree);
    cout << endl << generated_code << endl;

    ofstream file("tmp.cpp");
    if (file.is_open()) {
      file << generated_code;
      file.close();

      system("g++ tmp.cpp -o compiler && rm tmp.cpp");
    }
  }
}