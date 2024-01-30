%option never-interactive
%option nounput
%option noinput

%{
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "parser.h"

#define LIST_SOURCE                 appendToCurrentLine(yytext)
#define LIST_TOKEN(name)            do { LIST_SOURCE; if(opt_tok) printf("<%s>\n", name); } while(0)
#define LIST_LITERAL(name, literal) do { LIST_SOURCE; if(opt_tok) printf("<%s: %s>\n", name, literal); } while(0)
#define MAX_LINE_LENG               512

uint32_t line_num = 1;
char current_line[MAX_LINE_LENG];

static uint32_t opt_src = 1;
static uint32_t opt_tok = 1;
static char string_literal[MAX_LINE_LENG];
static char *current_line_ptr = current_line;

static void appendToCurrentLine(const char *yytext_ptr);

%}

letter     [a-zA-Z]
digit      [0-9]
id         {letter}({letter}|{digit})*
octal      0[0-7]+
integer    0|([1-9]{digit}*)
sci        (([1-9]{digit}*)|({integer}(\.(0|{digit}*[1-9]))))[eE][+-]?{integer}
real       {integer}(\.(0|{digit}*[1-9]))
string     \"([^\"\n]|\"\")*\"

%x COMMENT
%%

" "|\t { LIST_SOURCE; }
"/*" { LIST_SOURCE; BEGIN(COMMENT); }
<COMMENT>"*/" { LIST_SOURCE; BEGIN(INITIAL); }
<COMMENT>(.) { LIST_SOURCE; }
\/\/"&S+"(.*) { LIST_SOURCE; opt_src = 1; }
\/\/"&T+"(.*) { LIST_SOURCE; opt_tok = 1; }
\/\/"&S-"(.*) { LIST_SOURCE; opt_src = 0; }
\/\/"&T-"(.*) { LIST_SOURCE; opt_tok = 0; }
\/\/(.*) { LIST_SOURCE; }


"," { LIST_TOKEN(","); return ','; }
";" { LIST_TOKEN(";"); return ';'; }
":" { LIST_TOKEN(":"); return ':'; }
"(" { LIST_TOKEN("("); return '('; }
")" { LIST_TOKEN(")"); return (')'); }
"[" { LIST_TOKEN("["); return ('['); }
"]" { LIST_TOKEN("]"); return (']'); }

"+" { LIST_TOKEN("+"); return '+'; }
"-" { LIST_TOKEN("-"); return '-'; }
"*" { LIST_TOKEN("*"); return '*'; }
"/" { LIST_TOKEN("/"); return '/'; }
"mod" { LIST_TOKEN("mod"); return MOD; }
":=" { LIST_TOKEN(":="); return ASSIGN; }
"<" { LIST_TOKEN("<"); return '<'; }
"<=" { LIST_TOKEN("<="); return LE; }
"<>" { LIST_TOKEN("<>"); return NE; }
">=" { LIST_TOKEN(">="); return GE; }
">" { LIST_TOKEN(">"); return '>'; }
"=" { LIST_TOKEN("="); return '='; }
"and" {LIST_TOKEN("and"); return AND; }
"or" {LIST_TOKEN("or"); return OR; }
"not" {LIST_TOKEN("not"); return NOT; }

"var" { LIST_TOKEN("KWvar"); return KWvar; }
"def" { LIST_TOKEN("KWdef"); return KWdef; }
"array" { LIST_TOKEN("KWarray"); return KWarray; }
"of" { LIST_TOKEN("KWof"); return KWof; }
"boolean" { LIST_TOKEN("KWboolean"); return KWboolean; }
"integer" { LIST_TOKEN("KWinteger"); return KWinteger; }
"real" { LIST_TOKEN("KWreal"); return KWreal; }
"string" { LIST_TOKEN("KWstring"); return KWstring; }
"true" { LIST_TOKEN("KWtrue"); return KWtrue; }
"false" { LIST_TOKEN("KWfalse"); return KWfalse; }
"while" { LIST_TOKEN("KWwhile"); return KWwhile; }
"do" { LIST_TOKEN("KWdo"); return KWdo; }
"if" { LIST_TOKEN("KWif"); return KWif; }
"then" { LIST_TOKEN("KWthen"); return KWthen; }
"else" { LIST_TOKEN("KWelse"); return KWelse; }
"for" { LIST_TOKEN("KWfor"); return KWfor; }
"to" { LIST_TOKEN("KWto"); return KWto; }
"begin" { LIST_TOKEN("KWbegin"); return KWbegin; }
"end" { LIST_TOKEN("KWend"); return KWend; }
"print" { LIST_TOKEN("KWprint"); return KWprint; }
"read" { LIST_TOKEN("KWread"); return KWread; }
"return" { LIST_TOKEN("KWreturn"); return KWreturn; }

{id} { LIST_LITERAL("id", yytext); return ID; }
{octal} { LIST_LITERAL("oct_integer", yytext); return OCT_INTEGER; }
{integer} { LIST_LITERAL("integer", yytext); return INTEGER; }
{real} { LIST_LITERAL("float", yytext); return FLOAT; }
{sci} { LIST_LITERAL("scientific", yytext); return SCIENTIFIC; }
{string} { 
    // remove the first and last character
    char* tmp = malloc(strlen(yytext));
    strcpy(tmp, yytext);
    tmp[strlen(yytext) - 1] = '\0';
    char* res = malloc(strlen(tmp));
    int sz = strlen(tmp);
    int idx = 0;
    for(int i=1;i<sz;i++){
        if(tmp[i] != '"'){
            res[idx++] = tmp[i];
        } else {
            if(tmp[i+1] == '"'){
                res[idx++] = '"';
                i++;
            }
        }
    }
    res[idx] = '\0';
    LIST_LITERAL("string", res);
    return STRING;
}


<INITIAL,COMMENT>\n {
    if (opt_src) {
        printf("%d: %s\n", line_num, current_line);
    }
    ++line_num;
    current_line[0] = '\0';
    current_line_ptr = current_line;
}

    /* Catch the character which is not accepted by rules above */
. {
    printf("Error at line %d: bad character \"%s\"\n", line_num, yytext);
    exit(-1);
}

%%

static void appendToCurrentLine(const char *yytext_ptr) {
    while (*yytext_ptr) {
        *current_line_ptr = *yytext_ptr;
        ++current_line_ptr;
        ++yytext_ptr;
    }
    *current_line_ptr = '\0';
}
