%option noyywrap

%{
#include "expr.tab.h"
#include <stdio.h>
#include <stdlib.h>

int yyerror(const char *);
int yylex();

%}

%define parse.error verbose

%token TIPO_NUM COMANDO_PRINT COMANDO_READ ID TIPO_STRING
%token LOGICA_EQ LOGICA_NE LOGICA_LE LOGICA_GE LOGICA_LT LOGICA_GT LOGICA_AND LOGICA_OR LOGICA_NOT
%token COMANDO_IF COMANDO_ELSE COMANDO_WHILE COMANDO_RETURN
%token TIPO_VOID TIPO_INT TIPO_FLOAT TIPO_STRING
%left LOGICA_LT LOGICA_LE LOGICA_GT LOGICA_GE LOGICA_EQ LOGICA_NE LOGICA_AND LOGICA_OR LOGICA_NOT

%%

programa: 
                lista_funcoes bloco_principal 
                | bloco_principal
                ;

lista_funcoes: 
                lista_funcoes funcao 
                | funcao
                ;

funcao: 
                tipo_retorno ID '(' declaracao_parametros ')' bloco_principal 
                | tipo_retorno ID '(' ')' bloco_principal
                ;

tipo_retorno: 
                TIPO_VOID 
                | tipo
                ;

declaracao_parametros: 
                declaracao_parametros ',' parametro 
                | parametro
                ;

parametro: 
                tipo ID

bloco_principal: 
                '{' declaracoes lista_comando '}' 
                | '{' lista_comando '}'
                ;

declaracoes:
                declaracoes declaracao 
                | declaracao
                ;

declaracao: 
                tipo lista_id ';'
                ;

tipo: 
                TIPO_INT 
                | TIPO_FLOAT 
                | TIPO_STRING
                ;

lista_id: 
                lista_id ',' ID 
                | ID
                ;

bloco: 
                '{' lista_comando '}'
                ;

lista_comando: 
                lista_comando comando
                | comando
                ;

comando: 
                comando_if 
                | comando_while 
                | comando_atribuicao 
                | comando_escrita ';'
                | comando_leitura ';'
                | chamada_funcao ';'
                | comando_return ';'
                ;

comando_if: 
                COMANDO_IF '(' expressao_logica ')' bloco
                | COMANDO_IF '(' expressao_logica ')' bloco COMANDO_ELSE bloco
                ;

comando_while: 
                COMANDO_WHILE '(' expressao_logica ')' bloco
                ;

comando_atribuicao: 
                ID '=' expressao_aritmetica
                ;

comando_escrita: 
                COMANDO_PRINT '(' expressao_aritmetica ')'
                ;

comando_leitura: 
                COMANDO_READ '(' ID ')'
                ;

chamada_funcao: 
                ID '(' lista_parametros ')'
                ;

lista_parametros: 
                lista_parametros ',' expressao_aritmetica 
                | expressao_aritmetica
                ;

comando_return: 
                COMANDO_RETURN expressao_aritmetica
                | COMANDO_RETURN
                ;

expressao_logica: 
                '(' expressao_logica ')' {$$ = $2;}
                | expressao_relacional LOGICA_AND expressao_logica {$$ = $1 && $3;}
                | expressao_relacional LOGICA_OR expressao_logica {$$ = $1 || $3;}
                | LOGICA_NOT expressao_logica {$$ = !$2;}
                | expressao_relacional {$$ = $1;}
                ;

expressao_relacional: 
                expressao_aritmetica LOGICA_EQ expressao_aritmetica  {$$ = $1 == $3;}
                | expressao_aritmetica LOGICA_NE expressao_aritmetica {$$ = $1 != $3;}
                | expressao_aritmetica LOGICA_LT expressao_aritmetica {$$ = $1 < $3;}
                | expressao_aritmetica LOGICA_LE expressao_aritmetica {$$ = $1 <= $3;}
                | expressao_aritmetica LOGICA_GT expressao_aritmetica {$$ = $1 > $3;}
                | expressao_aritmetica LOGICA_GE expressao_aritmetica {$$ = $1 >= $3;}
                ;

expressao_aritmetica: 
                | fator
                | expressao_aritmetica '*' expressao_aritmetica {$$ = $1 * $3;}
                | expressao_aritmetica '/' expressao_aritmetica  { if ($3==0) yyerror("divisao por zero"); else $$ = $1 / $3; }
                | expressao_aritmetica '+' expressao_aritmetica {$$ = $1 + $3;}
                | expressao_aritmetica '-' expressao_aritmetica {$$ = $1 - $3;}
                | '-' expressao_aritmetica {$$ = -$2;}
                | '(' expressao_aritmetica ')' {$$ = $2;}
                ;

fator: 
                TIPO_NUM 
                | ID 
                | TIPO_STRING 
                ;

%%

int yyerror(const char *str) {
    fprintf(stderr, "%s\n", str);
    return 0;
}