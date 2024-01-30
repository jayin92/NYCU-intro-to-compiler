#include "AST/return.hpp"

// TODO
ReturnNode::ReturnNode(
    const uint32_t line,
    const uint32_t col,
    ExpressionNode *p_expression
) : AstNode{line, col}, expression(p_expression) {}

// TODO: You may use code snippets in AstDumper.cpp
void ReturnNode::print() {}

void ReturnNode::visitChildNodes(AstNodeVisitor &p_visitor) {
    if (expression != nullptr) {
        expression->accept(p_visitor);
    }
}
