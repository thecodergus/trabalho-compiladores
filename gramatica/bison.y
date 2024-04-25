%language "c"

%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


int yyerror(const char *);
int yylex();

%}

%union {
    int inteiro;
    float real;
    char *string;
}

%define parse.error verbose
%define parse.trace true

// Simbolos de comandos
%token  COMANDO_PRINT COMANDO_READ COMANDO_IF COMANDO_ELSE COMANDO_WHILE COMANDO_RETURN

// Simbolo de identificadores de variaveis e funcoes
%token <string>ID 
%token LITERAL

// Simbolo de tipos
%token TIPO_VOID 
%token <real> TIPO_FLOAT 
%token <inteiro> TIPO_INT 
%token <string >TIPO_STRING

// Simbolos de abrir e fechar chaves
%token SIMBOLO_ABRE_CHAVES SIMBOLO_FECHA_CHAVES

// Simbolos de operadores logicos
%token LOGICA_EQ LOGICA_NE LOGICA_LE LOGICA_GE LOGICA_LT LOGICA_GT LOGICA_AND LOGICA_OR LOGICA_NOT

// Simbolos de operadores aritmeticos
%token OPERADOR_MULTIPLICACAO OPERADOR_DIVISAO OPERADOR_SOMA OPERADOR_SUBTRACAO OPERADOR_POTENCIA

// Simbolo de atribuicao
%token SIMBOLO_ATRIBUICAO

// Simbolocos de abre e fecha parenteses
%token SIMBOLO_ABRE_PARENTESES SIMBOLO_FECHA_PARENTESES

// Simbolo de virgula e ponto e virgula
%token SIMBOLO_VIRGULA SIMBOLO_PONTO_VIRGULA

// Simbolo para erros
%token ERRO

// Simbolo de fim de programa
%token FIM 0 "Fim do arquivo"


// Operadores
%precedence LOGICA_EQ LOGICA_GT LOGICA_LT LOGICA_GE LOGICA_LE LOGICA_NE
%left OPERADOR_SOMA OPERADOR_SUBTRACAO

// Tokens não associativos
%nonassoc LOGICA_OR LOGICA_AND LOGICA_NOT
%%
Inicio: 
    Programa FIM
    ;

Programa:
    ListaFuncoes BlocoPrincipal
    | BlocoPrincipal
    ;

ListaFuncoes:
    ListaFuncoes Funcao
    | Funcao
    ;

Funcao:
    TipoRetorno ID SIMBOLO_ABRE_PARENTESES DeclaracaoParametros SIMBOLO_FECHA_PARENTESES BlocoPrincipal
    | TipoRetorno ID SIMBOLO_ABRE_PARENTESES SIMBOLO_FECHA_PARENTESES BlocoPrincipal
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
    ;

BlocoPrincipal:
    SIMBOLO_ABRE_CHAVES Declaracoes ListaComandos SIMBOLO_FECHA_CHAVES
    | SIMBOLO_ABRE_CHAVES ListaComandos SIMBOLO_FECHA_CHAVES
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
    | TIPO_FLOAT
    | TIPO_STRING
    ;

ListaId:
    ListaId SIMBOLO_VIRGULA ID
    | ID
    ;

Bloco:
    SIMBOLO_ABRE_CHAVES ListaComandos SIMBOLO_FECHA_CHAVES
    ;

ListaComandos:
    ListaComandos Comando
    | Comando SIMBOLO_PONTO_VIRGULA
    ;

Comando:
    ComandoSe
    | ComandoEnquanto
    | ComandoAtribuicao
    | ComandoEscrita
    | ComandoLeitura
    | ComandoRetorno
    | Retorno
    ;

Retorno:
    COMANDO_RETURN ExpressaoAritmetica
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
    ;

ComandoEscrita:
    COMANDO_PRINT SIMBOLO_ABRE_PARENTESES ExpressaoAritmetica SIMBOLO_FECHA_PARENTESES
    ;

ComandoLeitura:
    COMANDO_READ SIMBOLO_ABRE_PARENTESES ID SIMBOLO_FECHA_PARENTESES
    ;

ComandoRetorno:
    ChamaFuncao
    ;

ChamaFuncao:
    ID SIMBOLO_ABRE_PARENTESES ListaParametros SIMBOLO_FECHA_PARENTESES
    | ID SIMBOLO_ABRE_PARENTESES SIMBOLO_FECHA_PARENTESES
    ;

ListaParametros:
    ListaParametros SIMBOLO_VIRGULA ExpressaoAritmetica
    | ExpressaoAritmetica
    ;

ExpressaoAritmetica:
    ExpressaoAritmetica OPERADOR_SOMA ExpressaoAritmetica
    | ExpressaoAritmetica OPERADOR_SUBTRACAO ExpressaoAritmetica
    | Fator
    ;

Fator:
    Termo
    | Fator OPERADOR_MULTIPLICACAO Termo
    | Fator OPERADOR_DIVISAO Termo
    ;

Termo:
    ID
    | LITERAL
    

ExpressaoLogica:
    ExpressaoLogica LOGICA_OR ExpressaoAritmetica
    | ExpressaoLogica LOGICA_AND ExpressaoAritmetica
    | ExpressaoLogica LOGICA_EQ ExpressaoAritmetica
    | ExpressaoLogica LOGICA_NE ExpressaoAritmetica
    | ExpressaoLogica LOGICA_LT ExpressaoAritmetica
    | ExpressaoLogica LOGICA_LE ExpressaoAritmetica
    | ExpressaoLogica LOGICA_GT ExpressaoAritmetica
    | ExpressaoLogica LOGICA_GE ExpressaoAritmetica
    | LOGICA_NOT ExpressaoLogica
    | SIMBOLO_ABRE_PARENTESES ExpressaoLogica SIMBOLO_FECHA_PARENTESES
    | ExpressaoAritmetica
    ;
%%

int yyerror(const char *str) {
    fprintf(stderr, "%s\n", str);
    return 0;
}