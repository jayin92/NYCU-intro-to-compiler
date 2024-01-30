#ifndef AST_ASSIGNMENT_NODE_H
#define AST_ASSIGNMENT_NODE_H

#include "AST/ast.hpp"
#include "AST/expression.hpp"
#include "AST/VariableReference.hpp"

#include <memory>

class AssignmentNode final : public AstNode {
  private:
    

  public:
    std::unique_ptr<VariableReferenceNode> m_lvalue;
    std::unique_ptr<ExpressionNode> m_expr;
    ~AssignmentNode() = default;
    AssignmentNode(const uint32_t line, const uint32_t col,
                   VariableReferenceNode *p_var_ref, ExpressionNode *p_expr)
        : AstNode{line, col}, m_lvalue(p_var_ref), m_expr(p_expr){}

    void accept(AstNodeVisitor &p_visitor) override { p_visitor.visit(*this); }
    void visitChildNodes(AstNodeVisitor &p_visitor) override;

    PTypeSharedPtr getLvalueType() const {
        return m_lvalue->getTypeSharedPtr();
    }

    PTypeSharedPtr getExprType() const {
        return m_expr->getTypeSharedPtr();
    }

    VariableReferenceNode &getLvalue() const { return *m_lvalue.get(); }
    ExpressionNode &getExpr() const { return *m_expr.get(); }
};

#endif
