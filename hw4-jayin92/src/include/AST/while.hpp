#ifndef AST_WHILE_NODE_H
#define AST_WHILE_NODE_H

#include "AST/ast.hpp"
#include "AST/expression.hpp"
#include "AST/CompoundStatement.hpp"

#include <memory>

class WhileNode final : public AstNode {
  private:
    std::unique_ptr<ExpressionNode> m_condition;
    std::unique_ptr<CompoundStatementNode> m_body;

  public:
    ~WhileNode() = default;
    WhileNode(const uint32_t line, const uint32_t col,
              ExpressionNode *p_condition, CompoundStatementNode *p_body)
        : AstNode{line, col}, m_condition(p_condition), m_body(p_body){}

    void accept(AstNodeVisitor &p_visitor) override { p_visitor.visit(*this); }
    void visitChildNodes(AstNodeVisitor &p_visitor) override;
    PTypeSharedPtr getConditionType() const {
        return m_condition->getTypeSharedPtr();
    }
    ExpressionNode& getCondition() const { return *m_condition.get(); }
};

#endif
