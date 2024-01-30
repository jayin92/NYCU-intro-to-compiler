#ifndef __AST_ASSIGNMENT_NODE_H
#define __AST_ASSIGNMENT_NODE_H


#include "AST/ast.hpp"
#include "AST/expression.hpp"
#include "AST/VariableReference.hpp"
#include "visitor/AstNodeVisitor.hpp"

class AssignmentNode : public AstNode {
  public:
    AssignmentNode(
      const uint32_t line,
      const uint32_t col,
      VariableReferenceNode *p_lvalue,
      ExpressionNode *p_expr
    );
    ~AssignmentNode() = default;

    void print() override;
    void accept(AstNodeVisitor &p_visitor) override { p_visitor.visit(*this); }
    void visitChildNodes(AstNodeVisitor &p_visitor) override;

  private:
    VariableReferenceNode *lvalue;
    ExpressionNode *expr;
};

#endif
