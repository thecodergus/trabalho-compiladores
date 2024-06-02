%language "c"

%code requires{
#include "ast.h"
#include "astUtils.h"
#include "utils/cvector.h"
}

%{
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
%define api.value.type {AST}

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
%token CONSTANTE_STRING "Texto"

// Simbolos de abrir e fechar chaves
%token SIMBOLO_ABRE_CHAVES "{"
%token SIMBOLO_FECHA_CHAVES "}"

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
%token SIMBOLO_ATRIBUICAO "="

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
/*
%type <AST> InicioPrograma
%type <AST> Programa
%type <AST> ListaFuncoes
%type <AST> Funcao
%type <AST> TipoRetorno
%type <AST> DeclaracaoParametros
%type <AST> Parametro
%type <AST> BloboPrincipal
%type <AST>Declaracoes
%type <AST> Declaracao
%type <AST> Tipo
%type <AST> ListaId
%type <AST> Bloco
%type <AST> ListaComando
%type <AST> Comando
%type <AST> Retorno
%type <AST> ComandoSe
%type <AST> ComandoEnquanto
%type <AST> ComandoAtribuicao
%type <AST> ComandoEscrita
%type <AST> ComandoLeitura
%type <AST> ChamadaProc
%type <AST> ChamadaFuncao
%type <AST> ListaParametros
%type <AST> ExpressaoLogica
%type <AST> TermoLogico
%type <AST> ExpressaoRelacional
%type <AST> ExpressaoAritmetica
%type <AST> CONSTANTE_STRING
%type <AST> ID
%type <AST> CONSTANTE_INT
%type <AST> CONSTANTE_FLOAT
%type <AST> LOGICA_EQ
%type <AST> LOGICA_NE
%type <AST> LOGICA_LE
%type <AST> LOGICA_GE
%type <AST> LOGICA_LT
%type <AST> LOGICA_GT
%type <AST> LOGICA_AND
%type <AST> LOGICA_OR
%type <AST> LOGICA_NOT
%type <AST> OPERADOR_SOMA
%type <AST> OPERADOR_SUBTRACAO
%type <AST> OPERADOR_MULTIPLICACAO
%type <AST> OPERADOR_DIVISAO
%type <AST> OPERADOR_POTENCIA
%type <AST> SIMBOLO_ABRE_PARENTESES
%type <AST> SIMBOLO_FECHA_PARENTESES
%type <AST> SIMBOLO_VIRGULA
%type <AST> SIMBOLO_PONTO_VIRGULA
%type <AST> SIMBOLO_ABRE_CHAVES
%type <AST> SIMBOLO_FECHA_CHAVES
%type <AST> SIMBOLO_ATRIBUICAO
%type <AST> FIM
%type <AST> TIPO_VOID
%type <AST> TIPO_FLOAT
%type <AST> TIPO_INT
%type <AST> TIPO_STRING
%type <AST> COMANDO_IF
%type <AST> COMANDO_ELSE
%type <AST> COMANDO_WHILE
%type <AST> COMANDO_RETURN
%type <AST> COMANDO_PRINT
%type <AST> COMANDO_READ
*/



%%
// COMEÇO CODIGO COM BASE NA GRAMATICA DO PROFESSOR

InicioPrograma:
    Programa FIM{
        $$ = $1;
    }
    | FIM
    ;

Programa:
    ListaFuncoes BloboPrincipal{
        $$ = criar_programa($1.u.filhos, $2.u.filhos);
    }
    | BloboPrincipal{
        $$ = criar_programa(NULL, $1.u.filhos);
    }
    ;

ListaFuncoes:
    ListaFuncoes Funcao{
        $$ = criar_lista_funcoes($1, $2);
    }
    | Funcao{
        $$ = criar_lista_funcoes($$, $1);
    }
    ;

Funcao:
    TipoRetorno ID SIMBOLO_ABRE_PARENTESES DeclaracaoParametros SIMBOLO_FECHA_PARENTESES BloboPrincipal
    | TipoRetorno ID SIMBOLO_ABRE_PARENTESES SIMBOLO_FECHA_PARENTESES BloboPrincipal
    ;

TipoRetorno:
    Tipo
    | TIPO_VOID{
        $$ = criar_tipo(Void);
    }
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
    TIPO_INT{
        $$ = criar_tipo(Int);
    }
    | TIPO_STRING{
        $$ = criar_tipo(String);
    }
    | TIPO_FLOAT{
        $$ = criar_tipo(Float);
    }
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
    | COMANDO_RETURN CONSTANTE_STRING
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
    | ID SIMBOLO_ATRIBUICAO CONSTANTE_STRING
    ;

ComandoEscrita:
    COMANDO_PRINT SIMBOLO_ABRE_PARENTESES ExpressaoAritmetica SIMBOLO_FECHA_PARENTESES
    | COMANDO_PRINT SIMBOLO_ABRE_PARENTESES CONSTANTE_STRING SIMBOLO_FECHA_PARENTESES
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
    | ListaParametros SIMBOLO_VIRGULA CONSTANTE_STRING
    | ExpressaoAritmetica
    | CONSTANTE_STRING
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