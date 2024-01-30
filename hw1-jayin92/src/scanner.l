%{
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define LIST_SOURCE                 appendToCurrentLine(yytext)
#define LIST_TOKEN(name)            do { LIST_SOURCE; if(opt_tok) printf("<%s>\n", name); } while(0)
#define LIST_LITERAL(name, literal) do { LIST_SOURCE; if(opt_tok) printf("<%s: %s>\n", name, literal); } while(0)
#define MAX_LINE_LENG               512

uint32_t line_num = 1;
static char current_line[MAX_LINE_LENG];

static uint32_t opt_src = 1;
static uint32_t opt_tok = 1;
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


"," { LIST_TOKEN(","); }
";" { LIST_TOKEN(";"); }
":" { LIST_TOKEN(":"); }
"(" { LIST_TOKEN("("); }
")" { LIST_TOKEN(")"); }
"[" { LIST_TOKEN("["); }
"]" { LIST_TOKEN("]"); }

"+" { LIST_TOKEN("+"); }
"-" { LIST_TOKEN("-"); }
"*" { LIST_TOKEN("*"); }
"/" { LIST_TOKEN("/"); }
"mod" { LIST_TOKEN("mod"); }
":=" { LIST_TOKEN(":="); }
"<" { LIST_TOKEN("<"); }
"<=" { LIST_TOKEN("<="); }
"<>" { LIST_TOKEN("<>"); }
">=" { LIST_TOKEN(">="); }
">" { LIST_TOKEN(">"); }
"=" { LIST_TOKEN("="); }
"and" {LIST_TOKEN("and"); }
"or" {LIST_TOKEN("or"); }
"not" {LIST_TOKEN("not"); }

"var" { LIST_TOKEN("KWvar"); }
"def" { LIST_TOKEN("KWdef"); }
"array" { LIST_TOKEN("KWarray"); }
"of" { LIST_TOKEN("KWof"); }
"boolean" { LIST_TOKEN("KWboolean"); }
"integer" { LIST_TOKEN("KWinteger"); }
"real" { LIST_TOKEN("KWreal"); }
"string" { LIST_TOKEN("KWstring"); }
"true" { LIST_TOKEN("KWtrue"); }
"false" { LIST_TOKEN("KWfalse"); }
"while" { LIST_TOKEN("KWwhile"); }
"do" { LIST_TOKEN("KWdo"); }
"if" { LIST_TOKEN("KWif"); }
"then" { LIST_TOKEN("KWthen"); }
"else" { LIST_TOKEN("KWelse"); }
"for" { LIST_TOKEN("KWfor"); }
"to" { LIST_TOKEN("KWto"); }
"begin" { LIST_TOKEN("KWbegin"); }
"end" { LIST_TOKEN("KWend"); }
"print" { LIST_TOKEN("KWprint"); }
"read" { LIST_TOKEN("KWread"); }
"return" { LIST_TOKEN("KWreturn"); }

{id} { LIST_LITERAL("id", yytext); }
{octal} { LIST_LITERAL("oct_integer", yytext); }
{integer} { LIST_LITERAL("integer", yytext); }
{real} { LIST_LITERAL("float", yytext); }
{sci} { LIST_LITERAL("scientific", yytext); }
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

int main(int argc, const char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s [filename]\n", argv[0]);
        exit(-1);
    }

    yyin = fopen(argv[1], "r");
    if (yyin == NULL) {
        fprintf(stderr, "Open file error\n");
        exit(-1);
    }

    yylex();

    fclose(yyin);
    /* Free resources used by the scanner; otherwise, unused data structures
    will persist throughout the successive compiler phases. */
    yylex_destroy();

    return 0;
}
