#include "AST/program.hpp"

// TODO
ProgramNode::ProgramNode(
    const uint32_t line, const uint32_t col,
    const char *const p_name,
    const char *const p_return_type,
    std::vector<DeclNode*>* p_declarations,
    std::vector<FunctionNode*>* p_functions,
    CompoundStatementNode* p_compound_statement
)   : AstNode{line, col}, 
      name(p_name), 
      return_type(str2DataType(p_return_type)),
      declarations(p_declarations),
      functions(p_functions),
      compound_statement(p_compound_statement)
    {}


// void ProgramNode::print() {
//     // TODO
//     // outputIndentationSpace();

//     std::printf("program <line: %u, col: %u> %s %s\n",
//                 location.line, location.col,
//                 name.c_str(), "void");

//     // TODO
//     // incrementIndentation();
//     // visitChildNodes();
//     // decrementIndentation();
// }


void ProgramNode::visitChildNodes(AstNodeVisitor &p_visitor) { // visitor pattern version
    if(declarations != nullptr){
        for(auto &decl : *declarations){
            decl->accept(p_visitor);
        }
    }
    if(functions != nullptr){
        for(auto &func : *functions){
            func->accept(p_visitor);
        }
    }

    compound_statement->accept(p_visitor);
}

void ProgramNode::print() {}