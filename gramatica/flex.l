%option noyywrap

%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bison.tab.h"

#define yyterminate() return FIM
%}

delim     [ \t\r\n\f]+
digito    [0-9]
inteito    -?({digito})+
float       -?({digito})+("."{digito}*)?("e"[+-]?{digito}+)?
id        [a-zA-Z_][a-zA-Z0-9_]*
string    \"([^\\\n]|(\\.))*?\"

%%

if              { return COMANDO_IF; }
else            { return COMANDO_ELSE; }
while           { return COMANDO_WHILE; }
print           { return COMANDO_PRINT; }
read            { return COMANDO_READ; }
return          { return COMANDO_RETURN; }
void            { return TIPO_VOID; }
int             { return TIPO_INT; }
float           { return TIPO_FLOAT; }
string          { return TIPO_STRING; }
"=="            { return LOGICA_EQ; }
"!="            { return LOGICA_NE; }
"<="            { return LOGICA_LE; }
">="            { return LOGICA_GE; }
"<"             { return LOGICA_LT; }
">"             { return LOGICA_GT; }
"&&"            { return LOGICA_AND; }
"||"            { return LOGICA_OR; }
"!"             { return LOGICA_NOT; }
"="             { return SIMBOLO_ATRIBUICAO; }
"("             { return SIMBOLO_ABRE_PARENTESES; }
")"             { return SIMBOLO_FECHA_PARENTESES; }
"{"             { return SIMBOLO_ABRE_CHAVES; }
"}"             { return SIMBOLO_FECHA_CHAVES; }
";"             { return SIMBOLO_PONTO_VIRGULA; }
"*"             { return OPERADOR_MULTIPLICACAO; }
"/"             { return OPERADOR_DIVISAO; }
"+"             { return OPERADOR_SOMA; }
"-"             { return OPERADOR_SUBTRACAO; }
"^"             { return OPERADOR_POTENCIA; }
","             { return SIMBOLO_VIRGULA; }
{inteito}       { yylval.inteiro = atoi(yytext); return INT; }
{float}         { yylval.real = atof(yytext); return FLOAT; }
{id}            { yylval.string = strdup(yytext); return ID; }
{string}        { yylval.string = strdup(yytext + 1); return STRING; }
{delim}         {}
#*              {}
.               { printf("Caractere invalido: %c\\n", yytext[0]); return ERRO; }
<<EOF>>         { return FIM; }


%%