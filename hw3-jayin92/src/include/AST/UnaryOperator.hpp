#ifndef __AST_UNARY_OPERATOR_NODE_H
#define __AST_UNARY_OPERATOR_NODE_H

#include "AST/ast.hpp"
#include "visitor/AstNodeVisitor.hpp"
#include "AST/expression.hpp"

class UnaryOperatorNode : public ExpressionNode {
  public:
    UnaryOperatorNode(
      const uint32_t line,
      const uint32_t col,
      const char* p_op,
      ExpressionNode* p_expression
    );
    ~UnaryOperatorNode() = default;

    void print() override;
    void accept(AstNodeVisitor &p_visitor) override { p_visitor.visit(*this); }
    void visitChildNodes(AstNodeVisitor &p_visitor) override;
    std::string getOp() const { return op; };

  private:
    // TODO: operator, expression
    std::string op;
    ExpressionNode* expression;
};

#endif
