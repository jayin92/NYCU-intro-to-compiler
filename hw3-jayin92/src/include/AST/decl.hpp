#ifndef __AST_DECL_NODE_H
#define __AST_DECL_NODE_H

#include "AST/ast.hpp"
#include "AST/variable.hpp"

class DeclNode : public AstNode {
  public:
    // variable declaration
    DeclNode(
      const uint32_t line,
      const uint32_t col,
      std::vector<VariableNode*> *p_vars,
      const char *const p_type,
      std::vector<int> *p_dims
    );

    DeclNode(
      const uint32_t line,
      const uint32_t col,
      std::vector<VariableNode*> *p_vars,
      const char *const p_type,
      // std::vector<int> *p_dims,
      ConstantValueNode* p_constant
    );

    // constant variable declaration
    //DeclNode(const uint32_t, const uint32_t col
    //         /* TODO: identifiers, constant */);

    ~DeclNode() = default;

    void print() override;
    void accept(AstNodeVisitor &p_visitor) override { p_visitor.visit(*this); }
    void visitChildNodes(AstNodeVisitor &p_visitor) override;
    const char *getTypeCString() const {
      std::string res = dataType2Str(type);
      if(dims != nullptr && dims -> size() != 0){
        res += " ";
        for(auto i: *dims){
          res += "[" + std::to_string(i) + "]";
        }
      } 
      return res.c_str();
    }
    std::vector<VariableNode*>* getVariables() const { return variables; }

  private:
    // TODO: variables
    std::vector<VariableNode*>* variables;
    DataType type;
    std::vector<int>* dims;
    // ConstantValueNode* constant;


};

#endif
