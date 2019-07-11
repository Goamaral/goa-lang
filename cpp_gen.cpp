#include "cpp_gen.hpp"

extern tree_t tree;

static stringstream generated_code_stream = stringstream();
string generated_code;

void generate_identation(int identation) {
  for (int i = 0; i < identation; ++i) generated_code_stream << "  ";
}

void gen_code_node(tree_t node, int identation) {
  string datatype = token_type_string(node.datatype);
  transform(datatype.begin(), datatype.end(), datatype.begin(), ::tolower);
  list<tree_t>::iterator node_children_it = node.children.begin();

  switch (node.type) {
    case FDEF:
      generated_code_stream << datatype << " " << node.value << "(";
      gen_code_node(*(node_children_it++), identation); // Args
      generated_code_stream << ") {\n";
      gen_code_node(*node_children_it, identation + 1); // Body
      generated_code_stream << "}\n";
      break;

    case DECL:
      generate_identation(identation);
      generated_code_stream << datatype << " " << node.value;
      break;

    case ARGS:
      while (node_children_it != node.children.end()) {
        gen_code_node(*node_children_it, identation);
        ++node_children_it;

        if (node_children_it != node.children.end()) {
          generated_code_stream << ", ";
        }
      }
      break;

    case RETURN_N:
      generate_identation(identation);
      generated_code_stream << "return " << node.value;
      break;

    default:
      while (node_children_it != node.children.end()) {
        gen_code_node(*node_children_it, identation);
        generated_code_stream << ";\n";
        ++node_children_it;
      }
      break;
  }
}

string generate_code(tree_t node) {
  list<tree_t>::iterator node_children_it = node.children.begin();

  while (node_children_it != node.children.end()) {
    gen_code_node(*node_children_it, 0);

    ++node_children_it;
  }

  generated_code  = generated_code_stream.str();
  return generated_code;
}