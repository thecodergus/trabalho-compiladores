%language "c"

%code requires{
#include "ast.h"
#include "utils/cvector.h"
}

%{
#define YYPARSER
#define YYSTYPE AST*

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


int yyerror(const char *);
int yylex();

%}


// Configurações

%define parse.error verbose
%define parse.trace true

%start InicioPrograma


// Simbolos de comandos
%token COMANDO_PRINT "print"
%token COMANDO_READ "read"
%token COMANDO_IF "if"
%token COMANDO_ELSE "else"
%token COMANDO_WHILE "while"
%token COMANDO_RETURN "return"

// Simbolo de identificadores de variaveis e funcoes
%token ID 

// Simbolo de tipos
%token TIPO_VOID TIPO_FLOAT TIPO_INT TIPO_STRING
%token CONSTANTE_FLOAT "Ponto Flutuante"
%token CONSTANTE_INT "Inteiro"
%token LITERAL "Texto"

// Simbolos de abrir e fechar chaves
%token '{' "{"
%token '}' "}"

// Simbolos de operadores logicos
%token LOGICA_EQ "=="
%token LOGICA_NE "!="
%token LOGICA_LE "<="
%token LOGICA_GE ">="
%token LOGICA_LT "<"
%token LOGICA_GT ">"
%token LOGICA_AND "&&"
%token LOGICA_OR "||"
%token LOGICA_NOT "!"

// Simbolos de operadores aritmeticos
%token OPERADOR_MULTIPLICACAO "*"
%token OPERADOR_DIVISAO "/"
%token OPERADOR_SOMA "+"
%token OPERADOR_SUBTRACAO "-"
%token OPERADOR_POTENCIA "^"

// Simbolo de atribuicao
%token <token> SIMBOLO_ATRIBUICAO "="

// Simbolocos de abre e fecha parenteses
%token '(' "("
%token ')' ")"

// Simbolo de virgula e ponto e virgula
%token ',' ","
%token ';' ";"

// Simbolo para erros
%token ERRO

// Simbolo de fim de programa
%token FIM 0 "Fim do arquivo"


// Operadores
%left OPERADOR_SOMA OPERADOR_SUBTRACAO
%left OPERADOR_MULTIPLICACAO OPERADOR_DIVISAO
%right OPERADOR_POTENCIA

// Tipos e estruturas
%type <AST*> InicioPrograma
%type <AST*> Programa
%type <AST*> ListaFuncoes
%type <AST*> Funcao
%type <AST*> TipoRetorno
%type <AST*> DeclaracaoParametros
%type <AST*> Parametro
%type <AST*> BloboPrincipal
%type <AST*>Declaracoes
%type <AST*> Declaracao
%type <AST*> Tipo
%type <AST*> ListaId
%type <AST*> Bloco
%type <AST*> ListaComando
%type <AST*> Comando
%type <AST*> Retorno
%type <AST*> ComandoSe
%type <AST*> ComandoEnquanto
%type <AST*> ComandoAtribuicao
%type <AST*> ComandoEscrita
%type <AST*> ComandoLeitura
%type <AST*> ChamadaProc
%type <AST*> ChamadaFuncao
%type <AST*> ListaParametros
%type <AST*> ExpressaoLogica
%type <AST*> TermoLogico
%type <AST*> ExpressaoRelacional
%type <AST*> ExpressaoAritmetica
%type <AST*> TIPO_INT
%type <AST*> TIPO_STRING
%type <AST*> TIPO_FLOAT
%type <AST*> TIPO_VOID
%type <AST*> CONSTANTE_INT
%type <AST*> CONSTANTE_FLOAT
%type <AST*> LITERAL
%type <AST*> ID
%type <AST*> COMANDO_RETURN
%type <AST*> COMANDO_IF
%type <AST*> COMANDO_ELSE
%type <AST*> COMANDO_WHILE
%type <AST*> COMANDO_PRINT
%type <AST*> COMANDO_READ
%type <AST*> LOGICA_EQ
%type <AST*> LOGICA_NE
%type <AST*> LOGICA_LE
%type <AST*> LOGICA_GE
%type <AST*> LOGICA_LT
%type <AST*> LOGICA_GT
%type <AST*> LOGICA_AND
%type <AST*> LOGICA_OR
%type <AST*> LOGICA_NOT
%type <AST*> OPERADOR_SOMA
%type <AST*> OPERADOR_SUBTRACAO
%type <AST*> OPERADOR_MULTIPLICACAO
%type <AST*> OPERADOR_DIVISAO
%type <AST*> OPERADOR_POTENCIA



%%
// COMEÇO CODIGO COM BASE NA GRAMATICA DO PROFESSOR

InicioPrograma:
    Programa FIM{
        $$ = $1;
    }
    | FIM{
        $$ = NULL;
    }
    ;

Programa:
    ListaFuncoes BloboPrincipal
    | BloboPrincipal
    ;

ListaFuncoes:
    ListaFuncoes Funcao
    | Funcao
    ;

Funcao:
    TipoRetorno ID '(' DeclaracaoParametros ')' BloboPrincipal
    | TipoRetorno ID '(' ')' BloboPrincipal
    ;

TipoRetorno:
    Tipo
    | TIPO_VOID
    ;

DeclaracaoParametros:
    DeclaracaoParametros ',' Parametro
    | Parametro
    ;

Parametro:
    Tipo ID

BloboPrincipal:
    '{' Declaracoes ListaComando '}'
    | '{' ListaComando '}'
    ;

Declaracoes:
    Declaracoes Declaracao
    | Declaracao ';'
    ;

Declaracao:
    Tipo ListaId
    ;

Tipo: 
    TIPO_INT
    | TIPO_STRING
    | TIPO_FLOAT
    ;

ListaId:
    ListaId ',' ID
    | ID
    ;

Bloco: 
    '{' ListaComando '}'

ListaComando:
    ListaComando Comando
    | Comando
    ;

Comando:
    ComandoSe
    | ComandoEnquanto
    | ComandoAtribuicao ';'
    | ComandoEscrita ';'
    | ComandoLeitura ';'
    | ChamadaProc ';'
    | Retorno ';'
    ;

Retorno:
    COMANDO_RETURN ExpressaoAritmetica
    | COMANDO_RETURN LITERAL
    | COMANDO_RETURN
    ;

ComandoSe:
    COMANDO_IF '(' ExpressaoLogica ')' Bloco
    | COMANDO_IF '(' ExpressaoLogica ')' Bloco COMANDO_ELSE Bloco
    ;

ComandoEnquanto:
    COMANDO_WHILE '(' ExpressaoLogica ')' Bloco
    ;

ComandoAtribuicao:
    ID SIMBOLO_ATRIBUICAO ExpressaoAritmetica
    | ID SIMBOLO_ATRIBUICAO LITERAL
    ;

ComandoEscrita:
    COMANDO_PRINT '(' ExpressaoAritmetica ')'
    | COMANDO_PRINT '(' LITERAL ')'
    ;

ComandoLeitura:
    COMANDO_READ '(' ID ')'
    ;

ChamadaProc:
    ChamadaFuncao
    ;

ChamadaFuncao:
    ID '(' ListaParametros ')'
    | ID '(' ')'
    ;

ListaParametros:
    ListaParametros ',' ExpressaoAritmetica
    | ListaParametros ',' LITERAL
    | ExpressaoAritmetica
    | LITERAL
    ;

// FIM CODIGO COM BASE NA GRAMATICA DO PROFESSOR


ExpressaoLogica:
    ExpressaoLogica LOGICA_AND TermoLogico
    | ExpressaoLogica LOGICA_OR TermoLogico
    | TermoLogico
    ;

TermoLogico:
    LOGICA_NOT TermoLogico
    | ExpressaoRelacional
    | '(' ExpressaoLogica ')'
    ;

ExpressaoRelacional:
    ExpressaoAritmetica LOGICA_EQ ExpressaoAritmetica
    | ExpressaoAritmetica LOGICA_NE ExpressaoAritmetica
    | ExpressaoAritmetica LOGICA_LE ExpressaoAritmetica
    | ExpressaoAritmetica LOGICA_GE ExpressaoAritmetica
    | ExpressaoAritmetica LOGICA_LT ExpressaoAritmetica
    | ExpressaoAritmetica LOGICA_GT ExpressaoAritmetica
    ;

ExpressaoAritmetica:
    CONSTANTE_INT
    | CONSTANTE_FLOAT
    | ID
    | ChamadaFuncao
    | ExpressaoAritmetica OPERADOR_SOMA ExpressaoAritmetica
    | ExpressaoAritmetica OPERADOR_SUBTRACAO ExpressaoAritmetica
    | ExpressaoAritmetica OPERADOR_MULTIPLICACAO ExpressaoAritmetica
    | ExpressaoAritmetica OPERADOR_DIVISAO ExpressaoAritmetica
    | ExpressaoAritmetica OPERADOR_POTENCIA ExpressaoAritmetica
    | '(' ExpressaoAritmetica ')'
    ;
%%

int yyerror(const char *str) {
    fprintf(stderr, "%s\n", str);
    return 0;
}