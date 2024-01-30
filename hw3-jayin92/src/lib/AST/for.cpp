#include "AST/for.hpp"

// TODO
ForNode::ForNode(
    const uint32_t line,
    const uint32_t col,
    DeclNode *p_decl,
    AssignmentNode *p_assignment,
    ConstantValueNode *p_condition,
    CompoundStatementNode *p_body
) : AstNode{line, col}, decl(p_decl), assignment(p_assignment), condition(p_condition), body(p_body) {}

// TODO: You may use code snippets in AstDumper.cpp
void ForNode::print() {}

void ForNode::visitChildNodes(AstNodeVisitor &p_visitor) {
    if (decl != nullptr) {
        decl->accept(p_visitor);
    }
    if (assignment != nullptr) {
        assignment->accept(p_visitor);
    }
    if (condition != nullptr) {
        condition->accept(p_visitor);
    }
    if (body != nullptr) {
        body->accept(p_visitor);
    }
}
