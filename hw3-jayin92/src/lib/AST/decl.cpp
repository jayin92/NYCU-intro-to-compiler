#include "AST/decl.hpp"

// TODO
DeclNode::DeclNode(
    const uint32_t line,
    const uint32_t col,
    std::vector<VariableNode*> *p_vars,
    const char *const p_type,
    std::vector<int>* p_dims
) : AstNode{line, col}, type(str2DataType(p_type)), dims(p_dims) {
    variables = new std::vector<VariableNode*>();
    for(auto &var : *p_vars){
        variables->push_back(
            new VariableNode(
                var->getLocation().line,
                var->getLocation().col,
                var->getNameCString(),
                p_type,
                nullptr,
                p_dims
            )
        );
    }
}

DeclNode::DeclNode(
    const uint32_t line,
    const uint32_t col,
    std::vector<VariableNode*> *p_vars,
    const char *const p_type,
    ConstantValueNode* p_constant
): AstNode{line, col}, type(str2DataType(p_type)), dims(nullptr) {
    variables = new std::vector<VariableNode*>();
    for(auto &var : *p_vars){
        variables->push_back(
            new VariableNode(
                var->getLocation().line,
                var->getLocation().col,
                var->getNameCString(),
                p_type,
                p_constant,
                nullptr
            )
        );
    }
}


// TODO
//DeclNode::DeclNode(const uint32_t line, const uint32_t col)
//    : AstNode{line, col} {}

// TODO: You may use code snippets in AstDumper.cpp
void DeclNode::print() {}

void DeclNode::visitChildNodes(AstNodeVisitor &p_visitor) {
    // printf("DeclNode::visitChildNodes\n");
    if(variables != nullptr){
        for(auto &var : *variables){
            var->accept(p_visitor);
        }
    }
}
