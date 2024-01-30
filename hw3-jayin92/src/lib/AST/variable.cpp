#include "AST/variable.hpp"

// TODO
VariableNode::VariableNode(
    const uint32_t line,
    const uint32_t col,
    const char *const p_name,
    const char *const p_type,
    ConstantValueNode* p_constant,
    std::vector<int>* p_dims
): AstNode{line, col}, name(p_name), type(str2DataType(p_type)), constant(p_constant), dims(p_dims) {}

// TODO: You may use code snippets in AstDumper.cpp
void VariableNode::print() {}

void VariableNode::visitChildNodes(AstNodeVisitor &p_visitor) {
    if(constant != nullptr)
        constant->accept(p_visitor);
}
