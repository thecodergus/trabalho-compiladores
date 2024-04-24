%option noyywrap

%{
#define YYSTYPE double
#include "expr.tab.h"
#include <stdio.h>
#include <stdlib.h>

int yyerror(const char *);
int yylex();

%}

%define parse.error verbose

%token ARITIMETICA_ADD ARITIMETICA_SUB ARITIMETICA_MUL ARITIMETICA_DIV TNUM COMANDO_PRINT COMANDO_READ TID TSTRING
%token LOGICA_EQ LOGICA_NE LOGICA_LE LOGICA_GE LOGICA_LT LOGICA_GT LOGICA_AND LOGICA_OR LOGICA_NOT
%token COMANDO_IF COMANDO_ELSE COMANDO_WHILE COMANDO_RETURN 
%token TIPO_VOID TIPO_INT TIPO_FLOAT TIPO_STRING
%left ARITIMETICA_ADD ARITIMETICA_SUB
%left ARITIMETICA_MUL ARITIMETICA_DIV
%left LOGICA_LT LOGICA_LE LOGICA_GT LOGICA_GE
%left LOGICA_EQ LOGICA_NE
%left LOGICA_AND
%left LOGICA_OR
%right LOGICA_NOT

%%

programa: lista_funcoes bloco_principal | bloco_principal

lista_funcoes: lista_funcoes funcao | funcao

funcao: tipo_retorno TID TAPAR decl_parametros TFPAR bloco_principal | tipo_retorno TID TAPAR TFPAR bloco_principal

tipo_retorno: TIPO_VOID | TIPO_INT | TIPO_FLOAT

decl_parametros: decl_parametros TCOMMA parametro | parametro

parametro: tipo TID

bloco_principal: TLCURLY declaracoes lista_comando TRCURLY | TLCURLY lista_comando TRCURLY

declaracoes: declaracoes declaracao | declaracao

declaracao: tipo lista_id TSEMICOLON

tipo: TIPO_INT | TIPO_FLOAT | TIPO_STRING

lista_id: lista_id TCOMMA TID | TID

bloco: TLCURLY lista_comando TRCURLY

lista_comando: lista_comando comando TSEMICOLON | comando TSEMICOLON

comando: comando_if | comando_while | comando_atribuicao | comando_escrita | comando_leitura | chamada_funcao | comando_return

comando_if: COMANDO_IF TAPAR expressao_logica TFPAR bloco %prec COMANDO_IF
           | COMANDO_IF TAPAR expressao_logica TFPAR bloco COMANDO_ELSE bloco %prec COMANDO_ELSE

comando_while: COMANDO_WHILE TAPAR expressao_logica TFPAR bloco

comando_atribuicao: TID TASSIGN expressao_aritmetica

comando_escrita: COMANDO_PRINT TAPAR expressao_aritmetica TFPAR

comando_leitura: COMANDO_READ TAPAR TID TFPAR

chamada_funcao: TID TAPAR lista_parametros TFPAR

lista_parametros: lista_parametros TCOMMA expressao_aritmetica | expressao_aritmetica

comando_return: COMANDO_RETURN expressao_aritmetica

expressao_logica: expressao_relacional LOGICA_AND expressao_logica | expressao_relacional

expressao_relacional: expressao_aritmetica LOGICA_EQ expressao_aritmetica | expressao_aritmetica LOGICA_NE expressao_aritmetica | expressao_aritmetica LOGICA_LT expressao_aritmetica | expressao_aritmetica LOGICA_LE expressao_aritmetica | expressao_aritmetica LOGICA_GT expressao_aritmetica | expressao_aritmetica LOGICA_GE expressao_aritmetica

expressao_aritmetica: expressao_aritmetica ARITIMETICA_ADD termo | expressao_aritmetica ARITIMETICA_SUB termo | termo

termo: termo ARITIMETICA_MUL fator | termo ARITIMETICA_DIV fator | fator

fator: TNUM | TID | TSTRING | TAPAR expressao_aritmetica TFPAR

%%

int yyerror(const char *str) {
    fprintf(stderr, "%s\n", str);
    return 0;
}

int main(int argc, char **argv) {
    yyparse();
    return 0;
}
