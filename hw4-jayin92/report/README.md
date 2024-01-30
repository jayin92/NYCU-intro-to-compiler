# hw4 report

|||
|-:|:-|
|Name|李杰穎|
|ID|110550088|

## How much time did you spend on this project

Roughly spent 1 hour to read the specification and understand it. Spending another 10 hours for completing the semantic analyzer.


## Project overview

### Symbol Entry

A symbol entry stores the information for a symbol. In this project, I store the information below:
- `name`: the name of symbol
- `kind`: `program`, `function`, `parameter`, `variable`, `loop_var` and `constant`
- `type`: Type of the symbol
- `attribute`: Attribute of the symbol
- `has_error`: whether this symbol has semantic error

I also defined various of getter and setter for this project.

### Symbol Table

Symbol table stores many symbol entries. In symbol table, I defined `addSymbol`, `hasSymbol`, etc. And function for printing the table.

### Symbol Manager

Symbol manager stores many symbol table in the current scope, implemented by a stack. I implement a function for searching symbol in all symbol tables in the symbol manager. Also a function for checking the existence of `loop_var` for a given name.

### Semantic Analyzer

#### Symbol table related nodes

In `ProgramNode`, `FunctionNode`, `ForNode` and `CompoundStatementNode`, we first need to create a new symbol table, push it to the symbol manager, and running semantic check for the node and its children. After this, we pop the symbol table out of the manager, and print it. The stack structure maintains the scope relationship.

#### `DeclNode`

We only need to visit the children of `DeclNode`, e.g. multiple `VariableNode`s. No semantic check is needed for this node.

#### `VariableNode`
This node declare new variable, once there is no semantic error, we need to push the new symbol to the top symbol table.

The semantic check contains:
1. Redeclaration
2. `loop_var` check
3. Valid array (every dimensions > 0)

#### `ConstantValueNode`

Some `ConstantValueNode`s are used for set the constant value for `VariableNode`. In this circumstances, I used a boolean variable `set_constant` to indicate that the current `ConstantValueNode` is used for set the constant value, thus, we should modify the last entry in the top symbol table from `variable` to `constant` and set the attribute to the corresponding constant value.

#### `FunctionNode`

In order to complete the semantic check of `return` statement in a function, I add a boolean variable `in_procedure` to indicates whether this function is a procedure or not. Also another string `func_type` for recording the return type of given function. These two variables are useful when conducting semantic check in `returnNode`.

#### `CompoundStatementNode`

Because we shouldn't create new symbol table when the compound statement corresponds to parameters. Therefore, I declare a boolean variable `function_para` to indicate this compound statement corresponds to parameters or not. If `function_para` is set, it won't create new symbol table.

#### `PrintNode` 

This node check the expression type, if the expression's type is not scalar (i.e. `integer`, `real`, `boolean` and `string`), it should raise semantic error.


#### `BinaryOperatorNode`

This node check the operator and operands are correct. Also has the type coercion to implicitly convert the type.

It first visit the left and right operands, determine the type of operands.

#### `UnaryOperatorNode`

Similar to `BinaryOperatorNode`, the only difference is it only has one operand.

#### `FunctionInvocationNode`

I first determine the arguments' type from the attribute in the symbol table. Splitting the string using `,`. Then check the number of arguments and parameters and type of corresponding parameters and arguments.

#### `VariableReferenceNode`

This node conducts semantic check using the symbol table. Moreover, it also deals with the type when referencing an array.

#### `AssignmentNode` 

This node first check the correctness of `lvalue`, including kind and type. Then check the consistency of types between `lvalue` and `expression`. Type coercion is also done here.

#### `ReadNode`

This node checks whether the referenced variable is scalar or not.

#### `IfNode` and `WhileNode`

These two node check whether the condition is `boolean` or not.

#### `ForNode`

This node checks the redeclaration of `loop_var` and the incremental order of lower and upper bound.

#### `ReturnNode` 

This node first check whether the scope it lives in is a procedure or program. And then check the returned type is consistent with the function declaration. Type coercion is done here.

## What is the hardest you think in this project

I think that the hardest part is to deal with array reference, especially the type of referenced array. For example, assume we declare an array `int a[2][3][2]`, the type of `a[1]` is `integer [3][2]`. In order to derive the type of referenced array, we need to copy the dimensions vector in `PType`, starting from a specific index.

Also, binary and unary operation is also challenging. Due to the type coercion, and to correctly pass the type of a expression to its parent node.



## Feedback to T.A.s

1. Maybe more implementation hints can be given to us, like how to evaluate the expression type and pass it to the parent node.
2. Bonus points for advanced symbol table implementation, e.g. the hash table. It may encourage students to use more efficient data structure thus increase the semantic analyzer's performance.