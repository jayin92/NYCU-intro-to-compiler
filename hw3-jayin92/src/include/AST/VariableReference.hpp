#ifndef __AST_VARIABLE_REFERENCE_NODE_H
#define __AST_VARIABLE_REFERENCE_NODE_H

#include "AST/expression.hpp"
#include "visitor/AstNodeVisitor.hpp"

class VariableReferenceNode : public ExpressionNode {
  public:
    // normal reference
    VariableReferenceNode(
      const uint32_t line,
      const uint32_t col,
      const char* p_name
    );
    // array reference
    VariableReferenceNode(
      const uint32_t line,
      const uint32_t col,
      const char* p_name,
      std::vector<ExpressionNode *> *p_expressions
    );

    ~VariableReferenceNode() = default;

    void print() override;
    void accept(AstNodeVisitor &p_visitor) override { p_visitor.visit(*this); }
    void visitChildNodes(AstNodeVisitor &p_visitor) override;
    std::string getName() const { return name; }

  private:
    std::string name;
    std::vector<ExpressionNode *> *expressions;
};

#endif
