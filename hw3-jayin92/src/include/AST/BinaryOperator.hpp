#ifndef __AST_BINARY_OPERATOR_NODE_H
#define __AST_BINARY_OPERATOR_NODE_H

#include "AST/expression.hpp"
#include "visitor/AstNodeVisitor.hpp"

#include <memory>

class BinaryOperatorNode : public ExpressionNode {
  public:
    BinaryOperatorNode(
      const uint32_t line,
      const uint32_t col,
      const char* p_op,
      ExpressionNode* p_left,
      ExpressionNode* p_right
    );
    ~BinaryOperatorNode() = default;

    void print() override;
    void accept(AstNodeVisitor &p_visitor) override { p_visitor.visit(*this); }
    void visitChildNodes(AstNodeVisitor &p_visitor) override;
    std::string getOp() const { return op; };

  private:
    std::string op;
    ExpressionNode* left;
    ExpressionNode* right;
    
};

#endif
