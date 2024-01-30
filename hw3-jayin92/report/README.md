# hw3 report

|||
|-:|:-|
|Name|李杰穎|
|ID|110550088|

## How much time did you spend on this project

I spent roughly 2 hours to read the provided documents, including the specification and tutorial for visitor pattern.

After reading the documents, I spent roughly 8 hours finishing this project.

## Project overview

> Please describe the structure of your code and the ideas behind your implementation in an organized way.
> The point is to show us how you deal with the problems. It is not necessary to write a lot of words or paste all of your code here.

### Definition of each AstNodes

We can notice that every node is inherited from the base class `AstNode`. Therefore, every node has the location variable. 

To complete each `AstNode` (e.g. `ProgramNode`, `DeclNode`, ...), we need to define the constructor and member variables of them. And because we can only use POD in the union type of bison, the parameters in constructor should be some pointers to STL (e.g. vector of pointers to other nodes). After gaining an understanding of the high-level concept of AST, I designed and finished the definition of each node relatively quick.

Additionally, I directly employed the visitor pattern in my homework. I need to define two more functions, i.e. `void accept(AstNodeVisitor &)` and `void visitChildNodes(AstNodeVisitor &)`, the former one is for the visitor to visit this node, the function is identical to every class; the latter one is for recursively traverse the AST, and dump every nodes in AST.

Lastly, because the member variables of every node should be placed in private, following the concept of OOP, I also define the getter and setter for some member variables that need to be accessed or modified.

### Modification to `parser.y`

First, I used forward declarations in the `%code require` to declare the class that will be later used in the grammar. 

Second, I declare the type of each variables in the `%union`. This includes the variable the scanner used to store the data and the type for each variables in grammar.

Third, I define the type of every variable using `%type <[type of variables]> [variables]` or `%token <[type of variables]> [variables]`.

Lastly, the body of the parser includes the grammars and the corresponding code for constructing the AST for the program. The structure of code is based on my design to each nodes.

### Modification to `scanner.l`

The modification to `scanner.l` is quite small. We only save the value of literal constant to `yylval` with corresponding union type name.

## What is the hardest you think in this project

I think that it's pretty hard to design the underlying C++ structure of the AST. Because for most of the node, it will have either "has-a" or "has-many" structure to other nodes. Therefore, to complete the definition of the top-level nodes, we need to complete the definition for the lower-level nodes first. This make my development of this project longer than I expect.

Secondly, because we use pointer to pass the data in Bison. This make it a little bit hard to debug, especially the variable is wrongly passed. In this homework, I inserted lots of `printf` statements for debugging.

Third, the parser I wrote in the HW 2 is actually wrong. For example, `print -1` should be parsed as `print (NegativeSign)(1: integer)`. However, the grammar I defined in the last homework parsed this as `print (-1: integer)`. Therefore, I spend some time to deal with this issue.

Lastly, because the specification doesn't provide the high-level hierarchy for the AST. I need to imagine it in my brain, this makes me take more time to finish the homework.


## Feedback to T.A.s

> Not required, but bonus point may be given.

I think that it will be great to provide the class diagram of the AST nodes, to provide the high-level concept of the hierarchy AST nodes. This will make the student know what should they need to finished in this homework, and the relationship between each nodes.

And also, there has some inconsistency between the AST guideline and the comment in the code. However, I was not able to find those nodes, when I was writing this report. But I'm sure that these kinds of nodes exist. I remember that that issue is the code template saids that some nodes contains a `ExpressionNode`, however the guideline shows that it should contain a `ConstantValueNode`. This issue is minor, and not effect by progress much, though.