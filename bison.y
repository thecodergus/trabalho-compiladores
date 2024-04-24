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

%token ARITIMETICA_ADD ARITIMETICA_SUB ARITIMETICA_MUL ARITIMETICA_DIV NUM COMANDO_PRINT COMANDO_READ ID TSTRING
%token LOGICA_EQ LOGICA_NE LOGICA_LE LOGICA_GE LOGICA_LT LOGICA_GT LOGICA_AND LOGICA_OR LOGICA_NOT
%token COMANDO_IF COMANDO_ELSE COMANDO_WHILE COMANDO_RETURN
%token TIPO_VOID TIPO_INT TIPO_FLOAT TIPO_STRING
%token SIMBOLO_PARENTESES_INICIO SIMBOLO_PARENTESES_FIM SIMBOLO_PONTO_VIRGULA SIMBOLO_VIRGULA SIMBOLO_IGUAL SIMBOLO_ABRE_CHAVES SIMBOLO_FECHA_CHAVES
%left ARITIMETICA_ADD ARITIMETICA_SUB ARITIMETICA_MUL ARITIMETICA_DIV
%left LOGICA_LT LOGICA_LE LOGICA_GT LOGICA_GE LOGICA_EQ LOGICA_NE LOGICA_AND LOGICA_OR
%right LOGICA_NOT

%%

programa: lista_funcoes bloco_principal | bloco_principal

lista_funcoes: lista_funcoes funcao | funcao

funcao: tipo_retorno ID SIMBOLO_PARENTESES_INICIO decl_parametros SIMBOLO_PARENTESES_FIM bloco_principal | tipo_retorno ID SIMBOLO_PARENTESES_INICIO SIMBOLO_PARENTESES_FIM bloco_principal

tipo_retorno: TIPO_VOID 
            | TIPO_INT 
            | TIPO_FLOAT

decl_parametros: decl_parametros SIMBOLO_VIRGULA parametro 
                | parametro

parametro: tipo ID

bloco_principal: SIMBOLO_ABRE_CHAVES declaracoes lista_comando SIMBOLO_FECHA_CHAVES 
                | SIMBOLO_ABRE_CHAVES lista_comando SIMBOLO_FECHA_CHAVES

declaracoes: declaracoes declaracao 
            | declaracao

declaracao: tipo lista_id SIMBOLO_PONTO_VIRGULA

tipo: TIPO_INT 
    | TIPO_FLOAT 
    | TIPO_STRING

lista_id: lista_id SIMBOLO_VIRGULA ID 
        | ID

bloco: SIMBOLO_ABRE_CHAVES lista_comando SIMBOLO_FECHA_CHAVES

lista_comando: lista_comando comando SIMBOLO_PONTO_VIRGULA 
            | comando SIMBOLO_PONTO_VIRGULA

comando: comando_if 
        | comando_while 
        | comando_atribuicao 
        | comando_escrita 
        | comando_leitura 
        | chamada_funcao 
        | comando_return

comando_if: COMANDO_IF SIMBOLO_PARENTESES_INICIO expressao_logica SIMBOLO_PARENTESES_FIM bloco %prec COMANDO_IF
           | COMANDO_IF SIMBOLO_PARENTESES_INICIO expressao_logica SIMBOLO_PARENTESES_FIM bloco COMANDO_ELSE bloco %prec COMANDO_ELSE

comando_while: COMANDO_WHILE SIMBOLO_PARENTESES_INICIO expressao_logica SIMBOLO_PARENTESES_FIM bloco

comando_atribuicao: ID SIMBOLO_IGUAL expressao_aritmetica

comando_escrita: COMANDO_PRINT SIMBOLO_PARENTESES_INICIO expressao_aritmetica SIMBOLO_PARENTESES_FIM

comando_leitura: COMANDO_READ SIMBOLO_PARENTESES_INICIO ID SIMBOLO_PARENTESES_FIM

chamada_funcao: ID SIMBOLO_PARENTESES_INICIO lista_parametros SIMBOLO_PARENTESES_FIM

lista_parametros: lista_parametros SIMBOLO_VIRGULA expressao_aritmetica | expressao_aritmetica

comando_return: COMANDO_RETURN expressao_aritmetica

expressao_logica: expressao_relacional LOGICA_AND expressao_logica 
                | expressao_relacional
NUM
expressao_relacional: expressao_aritmetica LOGICA_EQ expressao_aritmetica 
                    | expressao_aritmetica LOGICA_NE expressao_aritmetica 
                    | expressao_aritmetica LOGICA_LT expressao_aritmetica 
                    | expressao_aritmetica LOGICA_LE expressao_aritmetica 
                    | expressao_aritmetica LOGICA_GT expressao_aritmetica 
                    | expressao_aritmetica LOGICA_GE expressao_aritmetica

expressao_aritmetica: expressao_aritmetica ARITIMETICA_ADD termo 
                    | expressao_aritmetica ARITIMETICA_SUB termo 
                    | termo

termo: termo ARITIMETICA_MUL fator 
    | termo ARITIMETICA_DIV fator 
    | fator

fator: NUM 
    | ID 
    | TSTRING 
    | SIMBOLO_PARENTESES_INICIO expressao_aritmetica SIMBOLO_PARENTESES_FIM

%%

int yyerror(const char *str) {
    fprintf(stderr, "%s\n", str);
    return 0;
}

int main(int argc, char **argv) {
    yyparse();
    return 0;
}
