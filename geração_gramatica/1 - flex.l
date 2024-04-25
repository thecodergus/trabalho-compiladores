%option noyywrap

%{
#define YYSTYPE double
#include "expr.tab.h"
#include <stdlib.h>
%}

delim     [ \t\n]
ws        {delim}+
digito    [0-9]
num       {digito}+("."{digito}*)?("e"[+-]?{digito}+)?
id        [a-zA-Z_][a-zA-Z0-9_]*
string    \"([^\\"]|\\")*\"

%%

{ws}          {}
{num}         { yylval = atof(yytext); return TIPO_NUM; }
{id}          { yylval = strdup(yytext); return ID; }
{string}      { yylval = strdup(yytext + 1); yylval_len = strlen(yylval) - 1; return TIPO_STRING; }
"=="          { return LOGICA_EQ; }
"!="          { return LOGICA_NE; }
"<="          { return LOGICA_LE; }
">="          { return LOGICA_GE; }
"<"           { return LOGICA_LT; }
">"           { return LOGICA_GT; }
"&&"          { return LOGICA_AND; }
"||"          { return LOGICA_OR; }
"!"           { return LOGICA_NOT; }
"if"          { return COMANDO_IF; }
"else"        { return COMANDO_ELSE; }
"while"       { return COMANDO_WHILE; }
"print"       { return COMANDO_PRINT; }
"read"        { return COMANDO_READ; }
"return"      { return COMANDO_RETURN; }
"void"        { return TIPO_VOID; }
"int"         { return TIPO_INT; }
"float"       { return TIPO_FLOAT; }
"string"      { return TIPO_STRING; }

.             { ECHO; }

%%

int yywrap() {
    return 1;
}