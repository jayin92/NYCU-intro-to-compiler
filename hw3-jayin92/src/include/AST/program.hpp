#ifndef AST_PROGRAM_NODE_H
#define AST_PROGRAM_NODE_H

#include "AST/ast.hpp"
#include "AST/decl.hpp"
#include "AST/function.hpp"
#include "AST/CompoundStatement.hpp"
#include "visitor/AstNodeVisitor.hpp"

#include <memory>

class ProgramNode final : public AstNode {
  private:
    std::string name;
    DataType return_type;
    std::vector<DeclNode*>* declarations;
    std::vector<FunctionNode*>* functions;
    CompoundStatementNode* compound_statement;
    // TODO: return type, declarations, functions, compound statement

  public:
    ~ProgramNode() = default;
    ProgramNode(
      const uint32_t line,
      const uint32_t col,
      const char *const p_name,
      const char *const p_return_type,
      std::vector<DeclNode*>* p_declarations,
      std::vector<FunctionNode*>* p_functions,
      CompoundStatementNode* p_compound_statement
    );

    const char *getNameCString() const { return name.c_str(); }
    void accept(AstNodeVisitor &p_visitor) override { p_visitor.visit(*this); }
    void visitChildNodes(AstNodeVisitor &p_visitor) override;
    void print() override;
};


#endif
