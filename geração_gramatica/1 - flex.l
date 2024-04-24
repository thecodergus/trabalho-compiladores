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
id        [a-zA-Z_][a-zA-Z0-9_]*
string    \"([^\\"]|\\")*\"

%%

{ws}          {}
"+"           { return ARITIMETICA_ADD; }
"-"           { return ARITIMETICA_SUB; }
"*"           { return ARITIMETICA_MUL; }
"/"           { return ARITIMETICA_DIV; }
"("           { return SIMBOLO_PARENTESES_INICIO; }
")"           { return SIMBOLO_PARENTESES_FIM; }
"\n"          { return QUEBRA_LINHA; }
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
";"           { return SIMBOLO_PONTO_VIRGULA; }
","           { return SIMBOLO_VIRGULA; }
"="           { return SIMBOLO_IGUAL; }
"{"           { return SIMBOLO_ABRE_CHAVES; }
"}"           { return SIMBOLO_FECHA_CHAVES; }
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