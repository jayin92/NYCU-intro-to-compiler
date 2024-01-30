#ifndef __AST_CONSTANT_VALUE_NODE_H
#define __AST_CONSTANT_VALUE_NODE_H

#include "visitor/AstNodeVisitor.hpp"
#include "AST/expression.hpp"

class ConstantValueNode : public ExpressionNode {
  public:
    ConstantValueNode(
      const uint32_t line,
      const uint32_t col,
      const char *const p_type,
      const char *const p_value
    );
    ~ConstantValueNode() = default;

    void print() override;
    void accept(AstNodeVisitor &p_visitor) override { p_visitor.visit(*this); }
    const char *getValueCString() const {
      // using %f when the number is real
      // using %d when the number is integer
      if(type == DataType::REAL){
        return std::to_string(std::stof(value)).c_str();
      }
      return value.c_str(); 
    }

  private:
    // TODO: constant value
    DataType type;
    std::string value;
};

#endif
