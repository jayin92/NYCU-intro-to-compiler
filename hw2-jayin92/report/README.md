# hw2 report

|||
|-:|:-|
|Name|李杰穎|
|ID|110550088|

## How much time did you spend on this project

I spend around 30 minutes to understand the syntax and usage of yacc, and spend another 90 minutes to complete the grammar described in the spec and debug.

## Project overview

### Scanner Modification

In homework 1, when we read a token, we only need to print that token out. However, in this homework, the scanner will need to interact with parser. Therefore, the scanner must return the token after read it. 

For example, for symbols like `+`, `*`, `<`, etc. The correspond lex rules needs to be rewritten as follow,

```
"+" { LIST_TOKEN("+"); return '+'; }
...
"*" { LIST_TOKEN("+"); return '*'; }
"<" { LIST_TOKEN("+"); return '<'; }
```

And for other tokens, like `:=`, `<=`, `<>`, `mod`. These kinds of tokens consists of two or more symbols, we can't directly return the token itself. Therefore, I directly returned token that will be later defined in parser.

```
"mod" { LIST_TOKEN("mod"); return MOD; }
":=" { LIST_TOKEN(":="); return ASSIGN; }
...
"<=" { LIST_TOKEN("<="); return LE; }
"<>" { LIST_TOKEN("<>"); return NE; }
```

For the keyword, like `var`, `array`, `if`, `else`. I also return the tokens defined in parser. It's noteworthy that every token has prefix "KW", make it easier to be recognized as a keyword.

```
"var" { LIST_TOKEN("KWvar"); return KWvar; }
"def" { LIST_TOKEN("KWdef"); return KWdef; }
...
"return" { LIST_TOKEN("KWreturn"); return KWreturn; }
```

Lastly, for identifier, numbers (including octal integer, decimal integer, scientific notation and floating number) and string. I return the token with corresponding name.

```
{id} { LIST_LITERAL("id", yytext); return ID; }
{octal} { LIST_LITERAL("oct_integer", yytext); return OCT_INTEGER; }
{integer} { LIST_LITERAL("integer", yytext); return INTEGER; }
```

And for comments, tab and space, because these tokens shouldn't be parsed, therefore we don't need to return any token.

### Scanner Declarations

In this section, I first declared token that used in scanner, including operators, keywords and type of variables.

And I also defined the associativity of `+`, `-`, `*`, `/`. This four binary operator is all left associative.

Last, I defined the start variable of grammar as `Program`.

### Scanner Rules

This section defined the production rules that defined the grammar described in spec. 


Before explaining the production rules, I want to mention that the scanner has already tokenized the program, so that we don't need to handle new line and indent. When writing production rules, just focus on the order of variables in production body.


#### Program Body

The entire program consists of ID, declarations, functions, compound statements and `end`. The corresponding rule is relatively simple to write.

`Program: ID ';' Decl Functions CompoundStmt KWend;`

#### Useful Variables

I defined three variables that will later be used in some production rules, which is integer (for both decimal integer and octal integer), literal constant and the type of scalar.

#### Function 

There has two type of functions, function declaration and function definition. However the first part of these two type is identical, `ID(Formal Args) : Scalar Type` or `ID(Formal Args)`. Therefore, I move the header as a separate variable.

I also defined the variable for list of identifiers, formal arguments in function and the list of functions definitions and declarations.

#### Variables and Constants

Firstly, I defined the `Type` variable, this will derive the string of valid type. For non-array type, it's pretty straight forward. For array type, we can notice that array type can nested, i.e. multi-dimension array. Therefore, I defined the variable as `Type: ScalarType | KWarray Integer KWof Type`.

For variables and constants, I just follow the spec and defined the corresponding rules.

#### Statements

There have multiple type of statements:

- Compound statements
- Simple statements
  - Assignment
  - Print
  - Read
- Conditional statements
- While statements
- For statements
- Return statements
- Function call statements

I defined the corresponding rules. I think this part is the most difficult part in the parser, because we need to handle various of statements, with different syntax. 

For example, the compound statement is enclosed using a pair of `begin` and `end`. And the first part of compound statement is always lines of declarations. This makes me need to write more variable than expected.

And positions of the semicolon in the production rule are also a little bit hard to handle. Wrong position will make the grammar has unexpected behavior. Moreover, some statements don't even require a semicolon, i.e. while statements and for statements. Make me need to deal with the position of semicolon more carefully.

#### Expression 

Expression is last part in the rules, it basically defined the literal constant, referenced variables, function call and arithmetic operations. It's pretty straight forward to defined the rules. The only thing needs to be noticed is that the order of production rules represents the precedence of operators, the first production rules has lowest precedence. Therefore, the `AND`, `OR`, `NOT` is the first three rules.


### Scanner Routines

This section defined `yyerror` and `main` function. And this two functions have provided to us, so we don't need to modify anything in this section.

The `yyerror` function is an error handler, defining the behavior when encountering parsing error.

`main` function defined how we open file and parsed the program.


## What is the hardest you think in this project

As I mentioned earlier, I think that defined the statements rule is the hardest part in this project. Because I need to notice about the position of semicolon, which is harder than I originally expected. Also, statements have lots of type, which also make the complexity of rules increased. 

## Feedback to T.A.s

I think that in the slide that introduced yacc, maybe this slide can provide the information about how to write the grammar that derived empty string. Because the derivation that involved empty string is very important for CFG.

Also, I think maybe the spec can be also improved, in term of notation. Take `var identifier_list: array integer_constant of type;`  as an example, I didn't notice that `of` is actually a keyword, I thought it's part of description. I found `of` is a keyword until I didn't pass the test case, and read the source code.
