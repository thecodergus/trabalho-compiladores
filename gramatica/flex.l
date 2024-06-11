%option noyywrap

%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bison.tab.h"
#include "ast.h"

#define yyterminate() return FIM
%}

delim     [ \t\r\n\f]+
digito    [0-9]
minuscula [a-z]
maiuscula [A-Z]
inteito    -?({digito})+
float       -?({digito})+("."{digito}*)?("e"[+-]?{digito}+)?
id        ({maiuscula}|{minuscula})({maiuscula}|{minuscula}|{digito}|-)*
string    (\")(.*)(\")

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
{inteito}       { 
                    yylval = criar_constante(yytext, Int);
                    return CONSTANTE_INT; 
                }
{float}         {  
                    
                    yylval = criar_constante(yytext, Float);
                    return CONSTANTE_FLOAT; 
                }
{id}            { 
                    yylval = criar_idenfier(strdup(yytext));
                    return ID; 
                }
{string}        {  
                    printf("STRING: %s\n", yytext);
                    yylval = criar_constante(strdup(yytext + 1), String);
                    return CONSTANTE_STRING; 
                }
{delim}         {}
#*              {}
<<EOF>>         { return FIM; }
.               { printf("Caractere invalido: %c\\n", yytext[0]); return ERRO; }


%%