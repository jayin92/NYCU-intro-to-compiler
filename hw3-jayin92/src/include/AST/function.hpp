#ifndef __AST_FUNCTION_NODE_H
#define __AST_FUNCTION_NODE_H

#include "AST/ast.hpp"
#include "AST/decl.hpp"
#include "AST/CompoundStatement.hpp"

class FunctionNode : public AstNode {
  public:
    FunctionNode(
      const uint32_t line, 
      const uint32_t col,
      const char *const p_name,
      std::vector<DeclNode*>* p_declarations,
      const char *const p_return_type,
      CompoundStatementNode* p_compound_statement
    );
    ~FunctionNode() = default;

    void print() override;
    void accept(AstNodeVisitor &p_visitor) override { p_visitor.visit(*this); }
    void visitChildNodes(AstNodeVisitor &p_visitor) override;

    const char *getNameCString() const { return name.c_str(); }
    std::string getPrototypeCString() const {
      std::string res = dataType2Str(return_type) + " (";
      if(declarations->size() != 0){
        for(auto i: *declarations){
          for(auto j: *(i -> getVariables())){
            res += j -> getTypeCString() + ", ";
          }
        }
        res.pop_back();
        res.pop_back();
      }
      res += ")";
      return res;
    }

    void setBody(CompoundStatementNode* p_compound_statement){
      compound_statement = p_compound_statement;
    }

  private:
    // TODO: name, declarations, return type, compound statement
    std::string name;
    std::vector<DeclNode*>* declarations;
    DataType return_type;
    CompoundStatementNode* compound_statement;
};

#endif
