#ifndef __AST_VARIABLE_NODE_H
#define __AST_VARIABLE_NODE_H


#include "AST/ast.hpp"
#include "AST/ConstantValue.hpp"
#include "visitor/AstNodeVisitor.hpp"


class VariableNode : public AstNode {
  public:
    VariableNode(
      const uint32_t line,
      const uint32_t col,
      const char *const p_name,
      const char *const p_type,
      ConstantValueNode* p_constant,
      std::vector<int>* p_dims
    );
    ~VariableNode() = default;

    void print() override;
    void accept(AstNodeVisitor &p_visitor) override { p_visitor.visit(*this); }
    void visitChildNodes(AstNodeVisitor &p_visitor) override;
    const char *getNameCString() const { return name.c_str(); }
    std::string getTypeCString() const {
      std::string res = dataType2Str(type);
      if(dims != nullptr && dims -> size() != 0){
        res += " ";
        for(auto i: *dims){
          res += "[" + std::to_string(i) + "]";
        }
      } 
      // printf("res: %s\n", res.c_str());
      return res;
    }
    // ConstantValueNode* getConstant() const { return constant; }

  private:
    // TODO: variable name, type, constant value
    std::string name;
    DataType type;
    ConstantValueNode* constant;
    std::vector<int>* dims;
    
};

#endif
