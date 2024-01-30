#include "AST/ConstantValue.hpp"

// TODO
ConstantValueNode::ConstantValueNode(
    const uint32_t line,
    const uint32_t col,
    const char *const p_type,
    const char *const p_value
) : ExpressionNode{line, col}, type(str2DataType(p_type)), value(p_value) {}

// TODO: You may use code snippets in AstDumper.cpp
void ConstantValueNode::print() {}
