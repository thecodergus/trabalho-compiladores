%language "c"

%code top{
#include "expr.tab.h"
#include <stdio.h>
#include <stdlib.h>

int yyerror(const char *);
int yylex();

%}

%define parse.error verbose

-- Simbolos de comandos
%token  COMANDO_PRINT COMANDO_READ COMANDO_IF COMANDO_ELSE COMANDO_WHILE COMANDO_RETURN

-- Simbolo de identificadores de variaveis e funcoes
%token ID

-- Simbolos logicios
%token LOGICA_EQ LOGICA_NE LOGICA_LE LOGICA_GE LOGICA_LT LOGICA_GT LOGICA_AND LOGICA_OR LOGICA_NOT

-- Simbbolos de tipos de variaveis e funcoes
%token TIPO_VOID TIPO_INT TIPO_FLOAT TIPO_STRING

-- Simbolos de operadores aritmeticos
%token OPERADOR_MULTIPLICACAO OPERADOR_DIVISAO OPERADOR_SOMA OPERADOR_SUBTRACAO OPERADOR_POTENCIA 

-- Simbolos de operadores de atribuicao
%token  SIMBOLO_ATRIBUICAO 

-- Simbolos de abrir e fechar parenteses
%tokern SIMBOLO_ABRE_PARENTESES SIMBOLO_FECHA_PARENTESES

-- Simbolos de abrir e fechar chaves
%token SIMBOLO_ABRE_CHAVES SIMBOLO_FECHA_CHAVES


%left LOGICA_LT LOGICA_LE LOGICA_GT LOGICA_GE LOGICA_EQ LOGICA_NE LOGICA_AND LOGICA_OR LOGICA_NOT
%left OPERADOR_SOMA OPERADOR_SUBTRACAO OPERADOR_MULTIPLICACAO OPERADOR_DIVISAO OPERADOR_POTENCIA
%precedence LOGICA_NOT
%%

%%

int yyerror(const char *str) {
    fprintf(stderr, "%s\n", str);
    return 0;
}