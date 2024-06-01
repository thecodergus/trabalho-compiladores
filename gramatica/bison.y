%language "c"

%code requires{
#include "ast.h"
#include "utils/cvector.h"
}

%{
#define YYPARSER
#define YYSTYPE Token*

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

%union{
    
}

%start InicioPrograma


// Simbolos de comandos
%token <token> COMANDO_PRINT "print"
%token <token> COMANDO_READ "read"
%token <token> COMANDO_IF "if"
%token <token> COMANDO_ELSE "else"
%token <token> COMANDO_WHILE "while"
%token <token> COMANDO_RETURN "return"

// Simbolo de identificadores de variaveis e funcoes
%token <string>ID 

// Simbolo de tipos
%token TIPO_VOID TIPO_FLOAT TIPO_INT TIPO_STRING
%token <real> CONSTANTE_FLOAT "Ponto Flutuante"
%token <inteiro> CONSTANTE_INT "Inteiro"
%token <string> LITERAL "Texto"

// Simbolos de abrir e fechar chaves
%token SIMBOLO_ABRE_CHAVES "{"
%token SIMBOLO_FECHA_CHAVES "}"

// Simbolos de operadores logicos
%token <token> LOGICA_EQ "=="
%token <token> LOGICA_NE "!="
%token <token> LOGICA_LE "<="
%token <token> LOGICA_GE ">="
%token <token> LOGICA_LT "<"
%token <token> LOGICA_GT ">"
%token <token> LOGICA_AND "&&"
%token <token> LOGICA_OR "||"
%token <token> LOGICA_NOT "!"

// Simbolos de operadores aritmeticos
%token <token> OPERADOR_MULTIPLICACAO "*"
%token <token> OPERADOR_DIVISAO "/"
%token <token> OPERADOR_SOMA "+"
%token <token> OPERADOR_SUBTRACAO "-"
%token <token> OPERADOR_POTENCIA "^"

// Simbolo de atribuicao
%token <token> SIMBOLO_ATRIBUICAO "="

// Simbolocos de abre e fecha parenteses
%token SIMBOLO_ABRE_PARENTESES "("
%token SIMBOLO_FECHA_PARENTESES ")"

// Simbolo de virgula e ponto e virgula
%token SIMBOLO_VIRGULA ","
%token SIMBOLO_PONTO_VIRGULA ";"

// Simbolo para erros
%token ERRO

// Simbolo de fim de programa
%token FIM 0 "Fim do arquivo"


// Operadores
%left OPERADOR_SOMA OPERADOR_SUBTRACAO
%left OPERADOR_MULTIPLICACAO OPERADOR_DIVISAO
%right OPERADOR_POTENCIA

// Tipos e estruturas



%%
// COMEÇO CODIGO COM BASE NA GRAMATICA DO PROFESSOR

InicioPrograma:
    Programa FIM
    | FIM
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
    TipoRetorno ID SIMBOLO_ABRE_PARENTESES DeclaracaoParametros SIMBOLO_FECHA_PARENTESES BloboPrincipal
    | TipoRetorno ID SIMBOLO_ABRE_PARENTESES SIMBOLO_FECHA_PARENTESES BloboPrincipal
    ;

TipoRetorno:
    Tipo
    | TIPO_VOID
    ;

DeclaracaoParametros:
    DeclaracaoParametros SIMBOLO_VIRGULA Parametro
    | Parametro
    ;

Parametro:
    Tipo ID

BloboPrincipal:
    SIMBOLO_ABRE_CHAVES Declaracoes ListaComando SIMBOLO_FECHA_CHAVES
    | SIMBOLO_ABRE_CHAVES ListaComando SIMBOLO_FECHA_CHAVES
    ;

Declaracoes:
    Declaracoes Declaracao
    | Declaracao SIMBOLO_PONTO_VIRGULA
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
    ListaId SIMBOLO_VIRGULA ID
    | ID
    ;

Bloco: 
    SIMBOLO_ABRE_CHAVES ListaComando SIMBOLO_FECHA_CHAVES

ListaComando:
    ListaComando Comando
    | Comando
    ;

Comando:
    ComandoSe
    | ComandoEnquanto
    | ComandoAtribuicao SIMBOLO_PONTO_VIRGULA
    | ComandoEscrita SIMBOLO_PONTO_VIRGULA
    | ComandoLeitura SIMBOLO_PONTO_VIRGULA
    | ChamadaProc SIMBOLO_PONTO_VIRGULA
    | Retorno SIMBOLO_PONTO_VIRGULA
    ;

Retorno:
    COMANDO_RETURN ExpressaoAritmetica
    | COMANDO_RETURN LITERAL
    | COMANDO_RETURN
    ;

ComandoSe:
    COMANDO_IF SIMBOLO_ABRE_PARENTESES ExpressaoLogica SIMBOLO_FECHA_PARENTESES Bloco
    | COMANDO_IF SIMBOLO_ABRE_PARENTESES ExpressaoLogica SIMBOLO_FECHA_PARENTESES Bloco COMANDO_ELSE Bloco
    ;

ComandoEnquanto:
    COMANDO_WHILE SIMBOLO_ABRE_PARENTESES ExpressaoLogica SIMBOLO_FECHA_PARENTESES Bloco
    ;

ComandoAtribuicao:
    ID SIMBOLO_ATRIBUICAO ExpressaoAritmetica
    | ID SIMBOLO_ATRIBUICAO LITERAL
    ;

ComandoEscrita:
    COMANDO_PRINT SIMBOLO_ABRE_PARENTESES ExpressaoAritmetica SIMBOLO_FECHA_PARENTESES
    | COMANDO_PRINT SIMBOLO_ABRE_PARENTESES LITERAL SIMBOLO_FECHA_PARENTESES
    ;

ComandoLeitura:
    COMANDO_READ SIMBOLO_ABRE_PARENTESES ID SIMBOLO_FECHA_PARENTESES
    ;

ChamadaProc:
    ChamadaFuncao
    ;

ChamadaFuncao:
    ID SIMBOLO_ABRE_PARENTESES ListaParametros SIMBOLO_FECHA_PARENTESES
    | ID SIMBOLO_ABRE_PARENTESES SIMBOLO_FECHA_PARENTESES
    ;

ListaParametros:
    ListaParametros SIMBOLO_VIRGULA ExpressaoAritmetica
    | ListaParametros SIMBOLO_VIRGULA LITERAL
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
    | SIMBOLO_ABRE_PARENTESES ExpressaoLogica SIMBOLO_FECHA_PARENTESES
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
    | SIMBOLO_ABRE_PARENTESES ExpressaoAritmetica SIMBOLO_FECHA_PARENTESES
    ;
%%

int yyerror(const char *str) {
    fprintf(stderr, "%s\n", str);
    return 0;
}