# hw1 report

|Field|Value|
|-:|:-|
|Name| 李杰穎 |
|ID| 110550088 |

## How much time did you spend on this project

I spent roughly 30 minutes to understand the syntax and usage of lex and spent another 2 hours to complete the scanner.

## Project overview

In this project, we only need to write the scanner, so we only need to modify `src/scanner.l`.

### Definition

1. The code has provided some useful function and variable, like `LIST_SOURCE`, `LIST_TOKEN`, `LIST_LITERAL`, `opt_src` and `opt_tok`. This part of code doesn't need to modify.
2. I pre-defined some useful regular expression for tokens, like `letter`, `digit`, `id`, `octal`, `integer`, etc.
3. At the last line, I declare exclusive start condition `COMMENT` using `%x`. Exclusive means that if this start condtion begin, then only the rules with `<COMMENT>` will be used. This is very useful when dealing with C-style comment, e.g. `/* ... */`.


### Transition Rules
1. The first part of rules contain rules for comments, pseudocomments, whitespace and tab. Because these kinds of rule should be matched first, I placed them in the first part of trainsition rules. Noted that the order of these rules is important, if the order is incorrect, it might have some unexpected behavior to the scanner. Because of rules are mismatched.
2. The second part contains a bunch of tokens, from punctuations like `,`, `;`, `:`, `(` to keyworkds like `var`, `def`, `if`, `then`. Rules in this part relatively simple, it even doesn't require regular expressions to match those token.
3. The last part contain rules for `id`, `oct_integer`, `integer`, `scientific`, etc. Rules here are more complicated, and needs to carefully desgined to match the spec. Additionally, I wrote some simple C code to process the string literal, I would describe it in the next section.


### User Subroutines

Code here doesn't need to modify. It basically define the main function and a helper function to copy the string from `yytext` to `current_line`. This is needed because the scanner needs to print the entire line after it parse a line.


## What is the hardest you think in this project

I think that hardest part in this project is to correctly arrange each rule. Because some rules needs to be match first, for example, the rules of comment and pseudocomments should be placed in the beginning.

It's also pretty hard to write the corresponding regular expression for each kinds of token. For example, the scientific notation has lots of detail need to be awared of, like `123.0E10` is a valid scientific notation. This is what I didn't notice at the first. Therefore, I modified the regular expression of scientific notation from `(([1-9]{digit}*)|({integer}(\.({digit}*[1-9]))))[eE][+-]?{integer}` to `(([1-9]{digit}*)|({integer}(\.(0|{digit}*[1-9]))))[eE][+-]?{integer}`.

Last but not the least, when dealing with `string`. I created two temporaily pointer to process the string, one for removing the double quotes in the original token, another is for removing the double quote for escaping the double quote. To do this, I write some simple lines of code to complete this. 

The above three is the hardest part in this project.


## Feedback to T.A.s

I think that the difference between `%s` and `%x` is worth mentioning in class. Using `%x` for start condition is much easier and more straightforward then using `%s` when writing the scanner for `/* ... */` style comments.

> Please help us improve our assignment, thanks.
