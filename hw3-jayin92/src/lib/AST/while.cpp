#include "AST/while.hpp"

// TODO
WhileNode::WhileNode(
    const uint32_t line,
    const uint32_t col,
    ExpressionNode *p_condition,
    CompoundStatementNode *p_body
)
    : AstNode{line, col}, condition(p_condition), body(p_body) {}

// TODO: You may use code snippets in AstDumper.cpp
void WhileNode::print() {}

void WhileNode::visitChildNodes(AstNodeVisitor &p_visitor) {
    if(condition != nullptr){
        condition->accept(p_visitor);
    }
    if(body != nullptr){
        body->accept(p_visitor);
    }
}
