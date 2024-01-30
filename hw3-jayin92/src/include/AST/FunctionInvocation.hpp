#ifndef __AST_FUNCTION_INVOCATION_NODE_H
#define __AST_FUNCTION_INVOCATION_NODE_H

#include "AST/expression.hpp"
#include "visitor/AstNodeVisitor.hpp"

class FunctionInvocationNode : public ExpressionNode {
  public:
    FunctionInvocationNode(
      const uint32_t line,
      const uint32_t col,
      const char *p_name,
      std::vector<ExpressionNode *> *p_expressions
    );
    ~FunctionInvocationNode() = default;

    const char *getNameCString() const { return name.c_str(); } ;

    void print() override;
    void accept(AstNodeVisitor &p_visitor) override { p_visitor.visit(*this); }
    void visitChildNodes(AstNodeVisitor &p_visitor) override;

  private:
    // TODO: function name, expressions
    std::string name;
    std::vector<ExpressionNode *> *expressions;
};

#endif
