#include "AST/if.hpp"

// TODO
IfNode::IfNode(
    const uint32_t line,
    const uint32_t col,
    ExpressionNode* p_condition,
    CompoundStatementNode* p_body,
    CompoundStatementNode* p_else_body
)
    : AstNode{line, col}, condition(p_condition), body(p_body), else_body(p_else_body) {}

// TODO: You may use code snippets in AstDumper.cpp
void IfNode::print() {}

void IfNode::visitChildNodes(AstNodeVisitor &p_visitor) {
    if(condition != nullptr){
        condition->accept(p_visitor);
    }
    if(body != nullptr){
        body->accept(p_visitor);
    }
    if(else_body != nullptr){
        else_body->accept(p_visitor);
    }
}
