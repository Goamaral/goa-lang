#include "cpp_gen.hpp"

extern tree_t tree;

static stringstream generated_head_stream = stringstream();
static stringstream generated_body_stream = stringstream();
string generated_code;

string datatype_string(token_type_t datatype) {
  switch (datatype) {
    case I32: return "int32_t";
    case VOID: return "void";
    default: return "unknown_t";
  }
}

void generate_identation(int identation) {
  for (int i = 0; i < identation; ++i) generated_body_stream << "  ";
}

void Core(int identation, tree_t call_node) {
  if (!call_node.value.compare("print")) {
    if (call_node.children.size() > 0) {
      generated_head_stream << "#include <iostream>\n";
      generate_identation(identation);
      generated_body_stream << "std::cout << ";
      list<tree_t>::iterator args_it = call_node.children.begin();
      while (args_it != call_node.children.end()) {
        generated_body_stream << args_it->value;
        ++args_it;

        if (args_it != call_node.children.end()) {
          generated_body_stream << " << ";
        }
      }
    }
  } else if (!call_node.value.compare("println")) {
    generated_head_stream << "#include <iostream>\n";
    generate_identation(identation);
    generated_body_stream << "std::cout << ";
    list<tree_t>::iterator args_it = call_node.children.begin();
    while (args_it != call_node.children.end()) {
      generated_body_stream << args_it->value;
      ++args_it;

      if (args_it != call_node.children.end()) {
        generated_body_stream << " << ";
      }
    }

    generated_body_stream << " << std::endl";
  }
}

void gen_code_node(tree_t node, int identation, bool close = false) {
  list<tree_t>::iterator node_children_it = node.children.begin();
  ifstream bind_file;

  switch (node.type) {
    case FDEF:
      generated_body_stream << endl << datatype_string(node.datatype) << " " << node.value << "(";
      gen_code_node(*(node_children_it++), identation); // Args
      generated_body_stream << ") {\n";
      gen_code_node(*node_children_it, identation + 1); // Body
      generated_body_stream << "}\n";
      break;

    case DECL:
      generate_identation(identation);
      generated_body_stream << datatype_string(node.datatype) << " " << node.value;
      if (close) generated_body_stream << ";\n";
      break;

    case ARGS:
      while (node_children_it != node.children.end()) {
        gen_code_node(*node_children_it, identation);
        ++node_children_it;

        if (node_children_it != node.children.end()) {
          generated_body_stream << ", ";
        }
      }
      break;

    case RETURN_N:
      generate_identation(identation);
      generated_body_stream << "return " << node.value;
      break;

    case CALL:
      generate_identation(identation);
      generated_body_stream << node.value << "(";
      while (node_children_it != node.children.end()) {
        gen_code_node(*node_children_it, identation);
        ++node_children_it;

        if (node_children_it != node.children.end()) {
          generated_body_stream << ", ";
        }
      }
      generated_body_stream << ")";
      break;

    case ARG:
      generated_body_stream << node.value;
      break;

    case BIND_N:
      bind_file.open(node.value + ".cpp");
      if (bind_file.is_open()) {
        stringstream file_buffer;

        file_buffer << bind_file.rdbuf();
        string file_code(file_buffer.str());

        bind_file.close();
        generated_head_stream << file_code << endl << endl;
      }
      break;

    case METHOD_CALL:
      if (!node.value.compare("Core")) Core(identation, *(node.children.begin()));
      break;

    default:
      while (node_children_it != node.children.end()) {
        gen_code_node(*node_children_it, identation);
        generated_body_stream << ";\n";
        ++node_children_it;
      }
      break;
  }
}

void gen_function_prototype(tree_t node) {
  generated_body_stream << datatype_string(node.datatype) << " " << node.value << "(";
  list<tree_t>::iterator args = node.children.begin();
  list<tree_t>::iterator args_it = args->children.begin();
  while (args_it != args->children.end()) {
    generated_body_stream << datatype_string(args_it->datatype);
    ++args_it;

    if (args_it != args->children.end()) {
      generated_body_stream << ", ";
    }
  }

  generated_body_stream << ");\n";
}

string generate_code(tree_t node) {
  list<tree_t>::iterator node_children_it = node.children.begin();

  while (node_children_it != node.children.end()) {
    if (node_children_it->type == FDEF) gen_function_prototype(*node_children_it);

    ++node_children_it;
  }
  generated_body_stream << endl;

  node_children_it = node.children.begin();

  while (node_children_it != node.children.end()) {
    gen_code_node(*node_children_it, 0, true);

    ++node_children_it;
  }

  generated_code = generated_head_stream.str() + "\n\n" + generated_body_stream.str();
  return generated_code;
}