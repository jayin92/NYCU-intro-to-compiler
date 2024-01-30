#include <AST/ast.hpp>

// string to DataType
DataType str2DataType(const char *const p_str) {
  if(p_str == nullptr) return DataType::VOID;
  std::string str(p_str);
//   printf("str2DataType: %s\n", str.c_str());
  if (str == "integer") {
    return DataType::INTEGER;
  } else if (str == "real") {
    return DataType::REAL;
  } else if (str == "boolean") {
    return DataType::BOOLEAN;
  } else if (str == "string") {
    return DataType::STRING;
  } else if (str == "void") {
    return DataType::VOID;
  }
  return DataType::VOID;
}

// DataType to string
std::string dataType2Str(const DataType &type) {
  if (type == DataType::INTEGER) {
    return "integer";
  } else if (type == DataType::REAL) {
    return "real";
  } else if (type == DataType::BOOLEAN) {
    return "boolean";
  } else if (type == DataType::STRING) {
    return "string";
  } else if (type == DataType::VOID){
    return "void";
  }
  return "";
}


// prevent the linker from complaining
AstNode::~AstNode() {}

AstNode::AstNode(const uint32_t line, const uint32_t col)
    : location(line, col) {}

const Location &AstNode::getLocation() const { return location; }
