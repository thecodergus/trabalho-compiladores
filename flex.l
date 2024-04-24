%option noyywrap

%{
#define YYSTYPE double
#include "expr.tab.h"
#include <stdlib.h>
%}

delim     [ \t]
ws        {delim}+
digito    [0-9]
num       {digito}+("."{digito}*)?("e"[+-]?{digito}+)?
id        [a-zA-Z][a-zA-Z0-9]*
string    \"([^\\"]|\\")*\"

%%

{ws}          {}
"+"           { return ARITIMETICA_ADD; }
"-"           { return ARITIMETICA_SUB; }
"*"           { return ARITIMETICA_MUL; }
"/"           { return ARITIMETICA_DIV; }
/* "("           { return TAPAR; } */
/* ")"           { return TFPAR; } */
/* "\n"          { return TFIM; } */
{num}         { yylval = atof(yytext); return TNUM; }
{id}          { yylval = strdup(yytext); return TID; }
{string}      { yylval = strdup(yytext + 1); yylval_len = strlen(yylval) - 1; return TSTRING; }
"=="          { return LOGICA_EQ; }
"!="          { return LOGICA_NE; }
"<="          { return LOGICA_LE; }
">="          { return LOGICA_GE; }
"<"           { return LOGICA_LT; }
">"           { return LOGICA_GT; }
"&&"          { return LOGICA_AND; }
"||"          { return LOGICA_OR; }
"!"           { return LOGICA_NOT; }
/* ";"           { return TSEMICOLON; } */
/* ","           { return TCOMMA; } */
/* "="           { return TASSIGN; } */
/* "{"           { return TLCURLY; } */
/* "}"           { return TRCURLY; } */
"if"          { return COMANDO_IF; }
"else"        { return COMANDO_ELSE; }
"while"       { return COMANDO_WHILE; }
"print"       { return COMANDO_PRINT; }
"read"        { return COMANDO_READ; }
"return"      { return COMANDO_RETURN; }
"void"        { return COMANDO_VOID; }
"int"         { return COMANDO_INT; }
"float"       { return COMANDO_FLOAT; }
"string"      { return COMANDO_STRING_TYPE; }

.             { ECHO; }

%%

int yywrap() {
    return 1;
}
