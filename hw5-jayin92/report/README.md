# hw5 report

|||
|-:|:-|
|Name|李杰穎|
|ID|110550088|

## How much time did you spend on this project

> e.g. 2 hours.

I spend around 1 hour to read the specification, and took 7 hours to complete the normal part, 5 hours to complete the bonus part.

## Project overview

In this project, we need to finish the code generator for generating RISC-V assembly code.

### `ProgramNode`

In this node, I mainly dump the instructions for file prologue, traverse variables and functions declaration nodes and compound statement node(main function). Finally, dump the instructions for declaring string and float number. 

I also dump the prologue and epilogue of main function in this node. 

### `DeclNode`

Just visit its child node, i.e. `VariableNode`

### `VariableNode`

There are several cases in `VariableNode`:

- Global Variables
  - Constant: add corresponding variable in `.section .rodata`
  - Variable: add `.comm a, 4, 4`
- Local Variables
  - First find the entry in symbol table, and set the correct offset of this variable in symbol table's entry.
  - Constant: evaluate the value and save it in `offset(s0)`
  - Normal variables: don't need to do anything
  - Function parameter: dump the instruction for copying the arguments to local stack

### `ConstantValueNode`

Evaluate the constant value, and push it to stack.

### `FunctionNode`

Mainly five steps for `FunctionNode`:
1. Dump function's prologue
2. Reset the offset (because the program will use a new stack)
3. Visit the declaration node (contains function's parameters)
4. Visit the body (a compound statement)
5. Dump function's epilogue

### `CompoundStatementNode`

Just visit all its children.

### `PrintNode`

1. Visit its children
2. Get the inferred type of expression for determining the correct print function
3. Special case for printing real number, we will need to load the value to `fa0` register.
4. Dump the print instruction


### `BinaryOperationNode`

1. Visit its children
2. Special case if the inferred type is real number
3. Write the corresponding instructions for binary operators
   1. Some of operators need more than one instruction to finish, because RISC-V does't support all possible operators.
4. Dump the final instruction

### `UnaryOperatorNode`

Similar to `BinaryOperatorNode`, but it only has one operand.

### `FunctionInvocationNode`

1. Visit its children
2. Pass the argument to `a0` ~ `a7`, if more than 8 arguments, use `t0`, `t1`, ...
3. Special case when argument is an array. Instead of pass the value, pass the address of that array. For example, if `a` is stored from `-12 + s0`, pass `-12+s0` as argument.
4. If the return type is not void, push the returned value to stack

### `VariableReferenceNode`

We need to dump different instructions for this node when it encounters in LHS or RHS of expression.

- LHS: We need to push the "address" to stack
- RHS: We need to push the "value" to stack

And special treatment for real number, array and string. I think this node is the most complex node in this project.

### `AssignmentNode`

1. Visit its children
2. Special case for string and real number
3. (real number and string) Push the information of variable name and value for dumping `.section .rodata` at the end of file
4. Dump the assign instructions

### `ReadNode`

1. Determine the correct function from inferred type
2. Dump read instructions

### `IfNode`

1. In order to deal with nested label problem, I preallocate the labels and save the label numbers in a vector. This technique is also used in `WhileNode` and `ForNode`.
2. Dump the condition statement's instruction
3. Dump the jump instruction
4. Dump the body statement's instruction
5. (If has else body) Dump the else statement's instruction
6. Dump ending label

### `WhileNode`

1. Dump starting label
2. Dump condition statement's instruction
3. Dump jump instruction
4. Dump body instruction
5. Dump instruction to jump back to stating label
6. Dump ending label

### `ForNode`

1. Visit loop_var's declaration node to fill the offset
2. Dump the correspond instructions

### `ReturnNode`

1. Get the value from stack, save it to `a0` and pop


## What is the hardest you think in this project

I feel that the most challenging part is to first understand the stack machine, and how to generate corresponding RISC-V code. However, once understanding it, it becomes relatively simple, because you just need to make sure that you always push the correct value to the stack, and remember to move the stack pointer. Because we don't need to optimize the code, we can just push every value to the stack, even if it will be used immediately. I think this greatly decrease the complexity of this project.

Also, debugging in this project is time-consuming, because we need to dive into the assembly code, and run the assembly in our human brain. This is not very easy, and sometimes I need to trace the code over and over again to find the bugs, and fix `CodeGenerator.cpp` accordingly.

Furthermore, the bonus part is quite challenging, too. As I mentioned earlier, I spend roughly the same time as normal part for bonus part. I put much effort to make sure that the load and save instructions for string and real work, because it needs special instructions.


## Feedback to T.A.s

First, I want to express my thanks to T.A.s for developing the homeworks, from homework 1 to homework 5. The specifications are very clear, and the test script is also very helpful.

As for the specification of this project, I feel like the specification for the bonus part can be clearer, for example, provided a complete example. 

For instance, when I writing the code generator for string, I didn't know where to put the declaration part. At first, I just dump it right in function, and found it's illegal in RISC-V after testing. 

Another example is `printReal`, I thought I can pass the argument by saving the value in `a0` for printing the real number, just as other print functions. However, I found it not working, and I took around 30 minutes to figure out RISC-V has a register `fa0` for passing the floating point argument. 

Furthermore, I don't get the description for array, in particular, 
> "For simplicity, you can pass the array variables by value."

Does this mean we can modify the symbol table and save the array variable in symbol table. However, if we implement using this method, how to pass the argument to function.

Finally, the link to the RISC-V ISA specification is broken, though we can just google it, fix the link in spec is always a good idea.
