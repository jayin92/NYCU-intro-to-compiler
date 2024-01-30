%{
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

extern int32_t line_num;    /* declared in scanner.l */
extern char current_line[]; /* declared in scanner.l */
extern FILE *yyin;          /* declared by lex */
extern char *yytext;        /* declared by lex */

extern int yylex(void);
static void yyerror(const char *msg);
extern int yylex_destroy(void);
%}

%token MOD ASSIGN LE NE GE AND OR NOT
%token KWvar KWdef KWarray KWof KWboolean KWinteger KWreal KWstring
%token KWtrue KWfalse KWwhile KWdo KWif KWthen KWelse KWfor KWto
%token KWbegin KWend KWprint KWread KWreturn
%token ID OCT_INTEGER INTEGER FLOAT SCIENTIFIC STRING  

%left '+' '-'
%left '*' '/'

%start Program
%%

// Program Body
Program: ID ';' Decl Functions CompoundStmt KWend;


// Useful Variables
Integer: INTEGER | OCT_INTEGER;
LiteralConst: Integer | FLOAT | SCIENTIFIC | KWtrue | KWfalse | STRING;
ScalarType: KWboolean | KWinteger | KWreal | KWstring;


// Functions
IDList: ID ',' IDList | ID;
FormalArg: IDList ':' ScalarType;
FormalArgs: FormalArg ';' FormalArgs 
          | FormalArg
          | %empty ;

Function : ID '(' FormalArgs ')' ':' ScalarType
         | ID '(' FormalArgs ')'

FunctionDec :  Function ';' ;
FunctionDef :  Function CompoundStmt KWend ;

Functions: FunctionDec Functions
         | FunctionDef Functions
         | %empty ;


// Variables and Constants
Type: ScalarType | KWarray Integer KWof Type;

Variable: KWvar IDList ':' ScalarType
        | KWvar IDList ':' KWarray Integer KWof Type ;

Const: KWvar IDList ':' LiteralConst ;


// Statements
CompoundStmt: KWbegin Decl Stmts KWend;

Decl: Const ';' Decl
    | Variable ';' Decl
    | %empty ;

Stmts: Stmt Stmts 
     | Stmt
     | %empty ;

Stmt: SimpleStmt ';'
    | CompoundStmt
    | ConditionalStmt
    | WhileStmt
    | ForStmt
    | ReturnStmt ';'
    | FunctionCall ';' ;

SimpleStmt: VarRef ASSIGN Expr
          | KWprint Expr 
          | KWread VarRef ;

VarRef: ID | ArrayRef ;
ArrayRef: ID Brackets;
Brackets: '[' Expr ']' Brackets | '[' Expr ']';

ConditionalStmt: KWif Expr KWthen CompoundStmt KWend KWif
               | KWif Expr KWthen CompoundStmt KWelse CompoundStmt KWend KWif;

WhileStmt: KWwhile Expr KWdo CompoundStmt KWend KWdo;      
ForStmt: KWfor ID ASSIGN Integer KWto Integer KWdo CompoundStmt KWend KWdo;
ReturnStmt: KWreturn Expr;
FunctionCall: ID '(' ExprList ')' | ID '(' ')';
ExprList: Expr ',' ExprList
        | Expr ;


// Expressions
Expr: Expr AND Expr
    | Expr OR Expr
    | NOT Expr
    | Expr '<' Expr
    | Expr '>' Expr
    | Expr LE Expr
    | Expr GE Expr
    | Expr NE Expr
    | Expr '=' Expr
    | Expr '+' Expr
    | Expr '-' Expr
    | Expr '/' Expr
    | Expr MOD Expr
    | Expr '*' Expr
    | '-' Expr
    | '(' Expr ')'
    | LiteralConst
    | VarRef
    | FunctionCall;

%%

void yyerror(const char *msg) {
    fprintf(stderr,
            "\n"
            "|-----------------------------------------------------------------"
            "---------\n"
            "| Error found in Line #%d: %s\n"
            "|\n"
            "| Unmatched token: %s\n"
            "|-----------------------------------------------------------------"
            "---------\n",
            line_num, current_line, yytext);
    exit(-1);
}

int main(int argc, const char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        exit(-1);
    }

    yyin = fopen(argv[1], "r");
    if (yyin == NULL) {
        perror("fopen() failed");
        exit(-1);
    }

    yyparse();

    fclose(yyin);
    yylex_destroy();

    printf("\n"
           "|--------------------------------|\n"
           "|  There is no syntactic error!  |\n"
           "|--------------------------------|\n");
    return 0;
}
