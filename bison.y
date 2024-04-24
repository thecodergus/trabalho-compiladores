%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
%}

%token TIPO_INT TIPO_FLOAT TIPO_STRING TIPO_VOID COMANDO_IF COMANDO_ELSE COMANDO_WHILE COMANDO_RETURN COMANDO_PRINT COMANDO_READ
%token CONSTANTE_INTEIRA CONSTANTE_FLOAT LITERAL ID
%left '+' '-'
%left '*' '/'
%left '<' '>' LE_GE NE_IGUAL
%left '&&'
%left '||'
%right '!'

%%

programa: lista_funcoes bloco_principal | bloco_principal

lista_funcoes: lista_funcoes funcao | funcao

funcao: tipo_retorno ID '(' decl_parametros ')' bloco_principal | tipo_retorno ID '(' ')' bloco_principal

tipo_retorno: tipo | TIPO_VOID

decl_parametros: decl_parametros ',' parametro | parametro

parametro: tipo ID

bloco_principal: '{' declaracoes lista_comando '}' | '{' lista_comando '}'

declaracoes: declaracoes declaracao | declaracao

tipo: TIPO_INT | TIPO_STRING | TIPO_FLOAT

lista_id: lista_id ',' ID | ID

bloco: '{' lista_comando '}'

lista_comando: lista_comando comando | comando

comando: comando_se | comando_enquanto | comando_atribuicao | comando_escrita | comando_leitura | chamada_proc | retorno

retorno: COMANDO_RETURN expressao_aritmetica ';' | COMANDO_RETURN LITERAL ';' | COMANDO_RETURN ';'

comando_se: COMANDO_IF '(' expressao_logica ')' bloco | COMANDO_IF '(' expressao_logica ')' bloco COMANDO_ELSE bloco

comando_enquanto: COMANDO_WHILE '(' expressao_logica ')' bloco

comando_atribuicao: ID '=' expressao_aritmetica ';' | ID '=' LITERAL ';'

comando_escrita: COMANDO_PRINT '(' expressao_aritmetica ')' ';' | COMANDO_PRINT '(' LITERAL ')' ';'

comando_leitura: COMANDO_READ '(' ID ')' ';'

chamada_proc: chamada_funcao ';'

chamada_funcao: ID '(' lista_parametros ')' | ID '(' ')'

lista_parametros: lista_parametros ',' expressao_aritmetica | lista_parametros ',' LITERAL | LITERAL

expressao_logica: expressao_logica '&&' expressao_relacional | expressao_logica '||' expressao_relacional | '!' expressao_relacional | expressao_relacional

expressao_relacional: expressao_aritmetica '<' expressao_aritmetica | expressao_aritmetica '>' expressao_aritmetica | expressao_aritmetica LE_GE expressao_aritmetica | expressao_aritmetica NE_IGUAL expressao_aritmetica

expressao_aritmetica: expressao_aritmetica '+' termo | expressao_aritmetica '-' termo | termo

termo: termo '*' fator | termo '/' fator | fator

fator: '(' expressao_aritmetica ')' | CONSTANTE_INTEIRA | CONSTANTE_FLOAT | ID | chamada_funcao

%%
