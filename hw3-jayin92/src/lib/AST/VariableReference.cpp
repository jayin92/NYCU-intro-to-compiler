#include "AST/VariableReference.hpp"

// normal reference
VariableReferenceNode::VariableReferenceNode(
    const uint32_t line,
    const uint32_t col,
    const char* p_name
)
  : ExpressionNode{line, col}, name(p_name), expressions(nullptr) {}
// array reference
VariableReferenceNode::VariableReferenceNode(
    const uint32_t line,
    const uint32_t col,
    const char* p_name,
    std::vector<ExpressionNode *> *p_expressions
)
  : ExpressionNode{line, col}, name(p_name), expressions(p_expressions) {}

// TODO: You may use code snippets in AstDumper.cpp
void VariableReferenceNode::print() {}

void VariableReferenceNode::visitChildNodes(AstNodeVisitor &p_visitor) {
    if(expressions != nullptr){
        for(auto &expression : *expressions){
            expression->accept(p_visitor);
        }
    }
}
