#include "AST/assignment.hpp"

// TODO
AssignmentNode::AssignmentNode(
    const uint32_t line,
    const uint32_t col,
    VariableReferenceNode *p_lvalue,
    ExpressionNode *p_expr
)
    : AstNode{line, col}, lvalue(p_lvalue), expr(p_expr) {}

// TODO: You may use code snippets in AstDumper.cpp
void AssignmentNode::print() {}

void AssignmentNode::visitChildNodes(AstNodeVisitor &p_visitor) {
    if(lvalue != nullptr){
        lvalue->accept(p_visitor);
    }
    if(expr != nullptr){
        expr->accept(p_visitor);
    }
}
