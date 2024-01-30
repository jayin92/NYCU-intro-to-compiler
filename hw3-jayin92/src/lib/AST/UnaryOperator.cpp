#include "AST/UnaryOperator.hpp"

// TODO
UnaryOperatorNode::UnaryOperatorNode(
    const uint32_t line,
    const uint32_t col,
    const char* p_op,
    ExpressionNode* p_expression
)
  : ExpressionNode{line, col}, op(p_op), expression(p_expression) {}

// TODO: You may use code snippets in AstDumper.cpp
void UnaryOperatorNode::print() {}

void UnaryOperatorNode::visitChildNodes(AstNodeVisitor &p_visitor) {
    if(expression != nullptr){
        expression->accept(p_visitor);
    }
}
