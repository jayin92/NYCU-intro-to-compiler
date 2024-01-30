%{
#include "AST/BinaryOperator.hpp"
#include "AST/CompoundStatement.hpp"
#include "AST/ConstantValue.hpp"
#include "AST/FunctionInvocation.hpp"
#include "AST/UnaryOperator.hpp"
#include "AST/VariableReference.hpp"
#include "AST/assignment.hpp"
#include "AST/ast.hpp"
#include "AST/decl.hpp"
#include "AST/expression.hpp"
#include "AST/for.hpp"
#include "AST/function.hpp"
#include "AST/if.hpp"
#include "AST/print.hpp"
#include "AST/program.hpp"
#include "AST/read.hpp"
#include "AST/return.hpp"
#include "AST/variable.hpp"
#include "AST/while.hpp"
#include "AST/AstDumper.hpp"

#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>

#define YYLTYPE yyltype

typedef struct YYLTYPE {
    uint32_t first_line;
    uint32_t first_column;
    uint32_t last_line;
    uint32_t last_column;
} yyltype;

extern int32_t line_num;    /* declared in scanner.l */
extern char current_line[]; /* declared in scanner.l */
extern FILE *yyin;          /* declared by lex */
extern char *yytext;        /* declared by lex */

static AstNode *root;

extern "C" int yylex(void);
static void yyerror(const char *msg);
extern int yylex_destroy(void);



%}

%code requires {
    class AstNode;
    class ProgramNode;
    class DeclNode;
    class FunctionNode;
    class VariableNode;
    class ConstantValueNode;
    class CompoundStatementNode;
    class PrintNode;
    class BinaryOperatorNode;
    class UnaryOperatorNode;
    class FunctionInvocationNode;
    class VariableReferenceNode;
    class AssignmentNode;
    class ReadNode;
    class IfNode;
    class WhileNode;
    class ForNode;
    class ReturnNode;
    class ExpressionNode;
    struct LiteralConstStruct {
        const char* type;
        const char* value;
    };
    struct StructureType {
        const char* type;
        std::vector<int>* dim;
    };
}

    /* For yylval */
%union {
    /* basic semantic value */
    char *identifier;
    char *val;
    struct LiteralConstStruct literal_const;
    struct StructureType structure_type;
    AstNode *node;
    ProgramNode *program_node;
    CompoundStatementNode *compound_stmt_ptr;
    PrintNode *print_node;
    BinaryOperatorNode *binary_op_node;
    UnaryOperatorNode *unary_op_node;
    FunctionInvocationNode *func_invocation_node;
    VariableReferenceNode *var_ref_node;
    AssignmentNode *assignment_node;
    ReadNode *read_node;
    IfNode *if_node;
    WhileNode *while_node;
    ForNode *for_node;
    ReturnNode *return_node;    
    DeclNode *decl_ptr;
    FunctionNode *func_ptr;
    ExpressionNode *expr;
    std::vector<VariableNode*> *variable_node;
    std::vector<DeclNode *> *decls_ptr;
    std::vector<FunctionNode *> *funcs_ptr;
    std::vector<AstNode *> *stmts_ptr;
    std::vector<char*>* id_list;
    std::vector<ExpressionNode *> *expr_list;
};

%type <program_node> Program
%type <decls_ptr> Decl FormalArgs
%type <func_ptr> Function FunctionDec FunctionDef
%type <funcs_ptr> Functions
%type <variable_node> IDList
%type <decl_ptr> Const Variable FormalArg
%type <literal_const> Integer
%type <literal_const> LiteralConst PosLiteralConst
%type <stmts_ptr> Stmts
%type <compound_stmt_ptr> CompoundStmt
%type <val> ScalarType
%type <structure_type> Type

%type <assignment_node> Assignment
%type <print_node> Print
%type <read_node> Read
%type <var_ref_node> VarRef ArrayRef
%type <if_node> ConditionalStmt
%type <expr_list> Brackets ExprList
%type <while_node> WhileStmt
%type <for_node> ForStmt
%type <return_node> ReturnStmt
%type <func_invocation_node> FunctionCall
%type <expr> Expr

%type <node> SimpleStmt Stmt


%token MOD ASSIGN LE NE GE AND OR NOT
%token KWvar KWdef KWarray KWof
%token <val> KWboolean KWinteger KWreal KWstring
%token KWwhile KWdo KWif KWthen KWelse KWfor KWto
%token KWbegin KWend KWprint KWread KWreturn
%token <identifier> ID 
%token <val> POS_OCT_INTEGER POS_INTEGER POS_FLOAT POS_SCIENTIFIC STRING KWtrue KWfalse

%left '+' '-'
%left '*' '/'

%start Program

%%



// Program Body
Program: ID ';' Decl Functions CompoundStmt KWend {
        // create a cstring 'void'
        root = new ProgramNode(
            @1.first_line,
            @1.first_column,
            $1,
            "void",
            $3,
            $4,
            $5
        );
        free($1);
};


// Useful Variables
Integer: 
      POS_INTEGER { $$.type = "integer"; $$.value = $1; }
    | POS_OCT_INTEGER {$$.type = "integer"; $$.value = $1; } ;

PosLiteralConst:
    POS_INTEGER { $$.type = "integer"; $$.value = $1; }
    | POS_OCT_INTEGER { $$.type = "integer"; $$.value = $1; }
    | POS_FLOAT { $$.type = "real"; $$.value = $1; }
    | POS_SCIENTIFIC { $$.type = "real"; $$.value = $1; }
    | KWtrue  { $$.type = "boolean"; $$.value = $1; }
    | KWfalse { $$.type = "boolean"; $$.value = $1; }
    | STRING { $$.type = "string"; $$.value = $1; } ;

LiteralConst:
    '-' PosLiteralConst {
        std::string str($2.value);
        str = '-' + str;
        $$.type = $2.type;
        $$.value = str.c_str();
    } | PosLiteralConst {
        $$ = $1;
    };

/* LiteralConst: Integer | FLOAT | SCIENTIFIC | KWtrue | KWfalse | STRING; */

ScalarType: 
      KWboolean { $$ = $1; }
    | KWinteger { $$ = $1; }
    | KWreal { $$ = $1; }
    | KWstring { $$ = $1; } ;


// Functions
IDList: 
    ID ',' IDList {
        $$ = new std::vector<VariableNode *>();
        $$->push_back(
            new VariableNode(
                @1.first_line,
                @1.first_column,
                $1,
                nullptr,
                nullptr,
                nullptr
            )
        );
        $$->insert($$->end(), $3->begin(), $3->end());
    } | 
    ID {
        $$ = new std::vector<VariableNode *>();
        $$->push_back(
            new VariableNode(
                @1.first_line,
                @1.first_column,
                $1,
                nullptr,
                nullptr,
                nullptr
            )
        );
    };

// IDList: ID ',' IDList | ID;


FormalArg: IDList ':' Type {
    std::vector<VariableNode*> *p_vars = new std::vector<VariableNode *>();
    for(auto &vars: *$1) {
        p_vars->push_back(
            new VariableNode(
                vars->getLocation().line,
                vars->getLocation().col,
                vars->getNameCString(),
                $3.type,
                nullptr,
                $3.dim
            )
        );
    }
    $$ = new DeclNode(
        @1.first_line,
        @1.first_column,
        p_vars,
        $3.type,
        $3.dim
    );

};
FormalArgs: FormalArg ';' FormalArgs { 
            $$ = new std::vector<DeclNode *>();
            $$->push_back($1);
            $$->insert($$->end(), $3->begin(), $3->end());
          }   
          | FormalArg { $$ = new std::vector<DeclNode *>(); $$->push_back($1); }
          | %empty { $$ = new std::vector<DeclNode *>(); } ;

Function : 
      ID '(' FormalArgs ')' ':' ScalarType {
        $$ = new FunctionNode(
            @1.first_line,
            @1.first_column,
            $1,
            $3,
            $6,
            nullptr
        );
      }
    | ID '(' FormalArgs ')' {
        // printf("not implemented\n");
        $$ = new FunctionNode(
            @1.first_line,
            @1.first_column,
            $1,
            $3,
            nullptr,
            nullptr
        );
      } ;

FunctionDec :  Function ';' { $$ = $1; } ;
FunctionDef :  Function CompoundStmt KWend {
    $1 -> setBody($2);
    $$ = $1;
};

Functions: 
      FunctionDec Functions {
        $$ = new std::vector<FunctionNode *>();
        $$->push_back($1);
        $$->insert($$->end(), $2->begin(), $2->end());
      }
    | FunctionDef Functions {
        $$ = new std::vector<FunctionNode *>();
        $$->push_back($1);
        $$->insert($$->end(), $2->begin(), $2->end());
    }
    | %empty { $$ = new std::vector<FunctionNode *>(); } ;


// Variables and Constants
Type: 
      ScalarType { $$.type = $1; $$.dim = new std::vector<int>(); }
    | KWarray Integer KWof Type {
        $$.type = $4.type;
        $$.dim = new std::vector<int>();
        $$.dim->push_back(atoi($2.value));
        $$.dim->insert($$.dim->end(), $4.dim->begin(), $4.dim->end());
    } ;

Variable: 
    KWvar IDList ':' ScalarType {
        std::vector<VariableNode*> *p_vars = new std::vector<VariableNode *>();
        for(auto &vars: *$2) {
            p_vars->push_back(
                new VariableNode(
                    vars->getLocation().line,
                    vars->getLocation().col,
                    vars->getNameCString(),
                    $4,
                    nullptr,
                    nullptr
                )
            );
        }
        $$ = new DeclNode(
            @1.first_line,
            @1.first_column,
            p_vars,
            $4,
            (std::vector<int>*)nullptr
        );
    } |
    KWvar IDList ':' KWarray Integer KWof Type {
        std::vector<int>* dim = new std::vector<int>();
        dim->push_back(atoi($5.value));
        dim->insert(dim->end(), $7.dim->begin(), $7.dim->end());
        // for(auto &d: *dim) {
        //     printf("dim: %d\n", d);
        // }
        std::vector<VariableNode*> *p_vars = new std::vector<VariableNode *>();
        for(auto &vars: *$2) {
            p_vars->push_back(
                new VariableNode(
                    vars->getLocation().line,
                    vars->getLocation().col,
                    vars->getNameCString(),
                    $7.type,
                    nullptr,
                    dim
                )
            );
        }
        $$ = new DeclNode(
            @1.first_line,
            @1.first_column,
            p_vars,
            $7.type,
            dim
        );
    };

Const: KWvar IDList ':' LiteralConst {

    ConstantValueNode *const_value = new ConstantValueNode(
        @4.first_line,
        @4.first_column,
        $4.type,
        $4.value
    );
    std::vector<VariableNode*> *p_vars = new std::vector<VariableNode *>();
    for(auto &vars: *$2) {
        p_vars->push_back(
            new VariableNode(
                vars->getLocation().line,
                vars->getLocation().col,
                vars->getNameCString(),
                $4.type,
                const_value,
                nullptr
            )
        );
    }
    $$ = new DeclNode(
        @1.first_line,
        @1.first_column,
        p_vars,
        $4.type,
        const_value
    );
    // printf("p_vars: %d\n", p_vars->size());

};


// Statements
CompoundStmt: KWbegin Decl Stmts KWend {
    $$ = new CompoundStatementNode(
        @1.first_line,
        @1.first_column,
        $2,
        $3
    );
};

Decl: Const ';' Decl { 
        $$ = new std::vector<DeclNode *>();
        $$->push_back($1);
        $$->insert($$->end(), $3->begin(), $3->end());
    }
    | Variable ';' Decl {
        $$ = new std::vector<DeclNode *>();
        $$->push_back($1);
        $$->insert($$->end(), $3->begin(), $3->end());
    }
    | %empty { $$ = new std::vector<DeclNode *>(); } ;

Stmts: 
    Stmt Stmts {
        $$ = new std::vector<AstNode *>();
        $$->push_back($1);
        $$->insert($$->end(), $2->begin(), $2->end());
    }
    | Stmt {
        $$ = new std::vector<AstNode *>();
        $$->push_back($1);
    }
    | %empty { $$ = new std::vector<AstNode *>(); } ;

Stmt: SimpleStmt ';' { $$ = $1; }
    | CompoundStmt { $$ = $1; }
    | ConditionalStmt { $$ = $1; }
    | WhileStmt { $$ = $1; }
    | ForStmt { $$ = $1; }
    | ReturnStmt ';' { $$ = $1; }
    | FunctionCall ';' { $$ = $1; } ;

SimpleStmt:
      Assignment { $$ = $1; }
    | Print { $$ = $1; }
    | Read  { $$ = $1; } ;

Assignment: VarRef ASSIGN Expr {
    $$ = new AssignmentNode(
        @2.first_line,
        @2.first_column,
        $1,
        $3
    );
};

Print: KWprint Expr {
    $$ = new PrintNode(
        @1.first_line,
        @1.first_column,
        $2
    );
};

Read: KWread VarRef {
    $$ = new ReadNode(
        @1.first_line,
        @1.first_column,
        $2
    );
};

VarRef: 
    ID { 
        $$ = new VariableReferenceNode(
            @1.first_line,
            @1.first_column,
            $1,
            nullptr
        );
    } | ArrayRef { $$ = $1; } ;

ArrayRef: ID Brackets {
    $$ = new VariableReferenceNode(
        @1.first_line,
        @1.first_column,
        $1,
        $2
    );
};

Brackets: 
    '[' Expr ']' Brackets {
        $$ = new std::vector<ExpressionNode *>();
        $$->push_back($2);
        $$->insert($$->end(), $4->begin(), $4->end());
    }
    | '[' Expr ']' {
        $$ = new std::vector<ExpressionNode *>();
        $$->push_back($2);
    } ;

ConditionalStmt: 
    KWif Expr KWthen CompoundStmt KWend KWif {
        $$ = new IfNode(
            @1.first_line,
            @1.first_column,
            $2,
            $4,
            nullptr
        );
    } 
    | KWif Expr KWthen CompoundStmt KWelse CompoundStmt KWend KWif {
        $$ = new IfNode(
            @1.first_line,
            @1.first_column,
            $2,
            $4,
            $6
        );
    } ;

WhileStmt: KWwhile Expr KWdo CompoundStmt KWend KWdo {
    $$ = new WhileNode(
        @1.first_line,
        @1.first_column,
        $2,
        $4
    );
};

ForStmt: KWfor ID ASSIGN Integer KWto Integer KWdo CompoundStmt KWend KWdo {
    VariableNode* var = new VariableNode(
        @2.first_line,
        @2.first_column,
        $2,
        nullptr,
        nullptr,
        nullptr
    );
    std::vector<VariableNode*> *p_vars = new std::vector<VariableNode *>();
    p_vars->push_back(var);
    DeclNode *init = new DeclNode(
        @2.first_line,
        @2.first_column,
        p_vars,
        "integer",
        (std::vector<int>*)nullptr
    );
    AssignmentNode *assign = new AssignmentNode(
        @3.first_line,
        @3.first_column,
        new VariableReferenceNode(
            @2.first_line,
            @2.first_column,
            $2,
            nullptr
        ),
        new ConstantValueNode(
            @4.first_line,
            @4.first_column,
            "integer",
            $4.value
        )
    );
    ConstantValueNode *end = new ConstantValueNode(
        @6.first_line,
        @6.first_column,
        "integer",
        $6.value
    );
    $$ = new ForNode(
        @1.first_line,
        @1.first_column,
        init,
        assign,
        end,
        $8
    );
};


ReturnStmt: KWreturn Expr {
    $$ = new ReturnNode(
        @1.first_line,
        @1.first_column,
        $2
    );
};

FunctionCall:
    ID '(' ExprList ')' {
        $$ = new FunctionInvocationNode(
            @1.first_line,
            @1.first_column,
            $1,
            $3
        );
    } | 
    ID '(' ')' {
        $$ = new FunctionInvocationNode(
            @1.first_line,
            @1.first_column,
            $1,
            nullptr
        );
    } ;

ExprList: 
    Expr ',' ExprList {
        $$ = new std::vector<ExpressionNode *>();
        $$->push_back($1);
        $$->insert($$->end(), $3->begin(), $3->end());
    } | 
    Expr {
        $$ = new std::vector<ExpressionNode *>();
        $$->push_back($1);
    } ;


// Expressions
Expr: 
    Expr AND Expr {
        $$ = new BinaryOperatorNode(
            @2.first_line,
            @2.first_column,
            "and",
            $1,
            $3
        );
        }
    | Expr OR Expr {
        $$ = new BinaryOperatorNode(
            @2.first_line,
            @2.first_column,
            "or",
            $1,
            $3
        );
        }
    | NOT Expr {
        $$ = new UnaryOperatorNode(
            @1.first_line,
            @1.first_column,
            "not",
            $2
        );
        }
    | Expr '<' Expr {
        $$ = new BinaryOperatorNode(
            @2.first_line,
            @2.first_column,
            "<",
            $1,
            $3
        );
        }
    | Expr '>' Expr {
        $$ = new BinaryOperatorNode(
            @2.first_line,
            @2.first_column,
            ">",
            $1,
            $3
        );
        }
    | Expr LE Expr {
        $$ = new BinaryOperatorNode(
            @2.first_line,
            @2.first_column,
            "<=",
            $1,
            $3
        );
        }
    | Expr GE Expr {
        $$ = new BinaryOperatorNode(
            @2.first_line,
            @2.first_column,
            ">=",
            $1,
            $3
        );
        }
    | Expr NE Expr {
        $$ = new BinaryOperatorNode(
            @2.first_line,
            @2.first_column,
            "<>",
            $1,
            $3
        );
        }
    | Expr '=' Expr {
        $$ = new BinaryOperatorNode(
            @2.first_line,
            @2.first_column,
            "=",
            $1,
            $3
        );
    }
    | Expr '+' Expr {
        $$ = new BinaryOperatorNode(
            @2.first_line,
            @2.first_column,
            "+",
            $1,
            $3
        );
    }
    | Expr '-' Expr {
        $$ = new BinaryOperatorNode(
            @2.first_line,
            @2.first_column,
            "-",
            $1,
            $3
        );
    }
    | Expr '/' Expr {
        $$ = new BinaryOperatorNode(
            @2.first_line,
            @2.first_column,
            "/",
            $1,
            $3
        );   
    }
    | Expr MOD Expr {
        $$ = new BinaryOperatorNode(
            @2.first_line,
            @2.first_column,
            "mod",
            $1,
            $3
        );
    }
    | Expr '*' Expr {
        $$ = new BinaryOperatorNode(
            @2.first_line,
            @2.first_column,
            "*",
            $1,
            $3
        );
    }
    | '-' Expr %prec '*' {
        $$ = new UnaryOperatorNode(
            @1.first_line,
            @1.first_column,
            "neg",
            $2
        );
    }
    | '(' Expr ')' {
        $$ = $2;
    }
    | PosLiteralConst {
        $$ = new ConstantValueNode(
            @1.first_line,
            @1.first_column,
            $1.type,
            $1.value
        );
    }
    | VarRef { $$ = $1; }
    | FunctionCall { $$ = $1; } ;

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
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <filename> [--dump-ast]\n", argv[0]);
        exit(-1);
    }

    yyin = fopen(argv[1], "r");
    if (yyin == NULL) {
        perror("fopen() failed");
        exit(-1);
    }

    yyparse();

    if (argc >= 3 && strcmp(argv[2], "--dump-ast") == 0) {
        /* printf("test\n"); */
        AstDumper ast_dumper;
        root->accept(ast_dumper);
    }

    printf("\n"
           "|--------------------------------|\n"
           "|  There is no syntactic error!  |\n"
           "|--------------------------------|\n");

    delete root;
    fclose(yyin);
    yylex_destroy();
    return 0;
}
