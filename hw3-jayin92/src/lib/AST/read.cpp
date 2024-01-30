#include "AST/read.hpp"

// TODO
ReadNode::ReadNode(
    const uint32_t line,
    const uint32_t col,
    VariableReferenceNode *p_target
)
    : AstNode{line, col}, target(p_target) {}

// TODO: You may use code snippets in AstDumper.cpp
void ReadNode::print() {}

void ReadNode::visitChildNodes(AstNodeVisitor &p_visitor) {
    if(target != nullptr){
        target->accept(p_visitor);
    }
}
