#include "AST/function.hpp"

// TODO
FunctionNode::FunctionNode(
    const uint32_t line, 
    const uint32_t col,
    const char *const p_name,
    std::vector<DeclNode*>* p_declarations,
    const char *const p_return_type,
    CompoundStatementNode* p_compound_statement
) : AstNode{line, col},
    name(p_name),
    declarations(p_declarations),
    return_type(str2DataType(p_return_type)),
    compound_statement(p_compound_statement) 
{}

// TODO: You may use code snippets in AstDumper.cpp
void FunctionNode::print() {}

void FunctionNode::visitChildNodes(AstNodeVisitor &p_visitor) {
    for(auto &decl : *declarations){
        decl->accept(p_visitor);
    }
    if(compound_statement != nullptr)
        compound_statement->accept(p_visitor);
}
