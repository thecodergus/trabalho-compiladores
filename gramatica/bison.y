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

// Simbolo de tipos
%token TIPO_VOID TIPO_FLOAT TIPO_INT TIPO_STRING
%token <real> CONSTANTE_FLOAT 
%token <inteiro> CONSTANTE_INT 
%token <string> LITERAL

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
%left LOGICA_OR
%left LOGICA_AND
%left LOGICA_EQ LOGICA_NE
%left LOGICA_LT LOGICA_GT LOGICA_LE LOGICA_GE
%left OPERADOR_SOMA OPERADOR_SUBTRACAO
%left OPERADOR_MULTIPLICACAO OPERADOR_DIVISAO
%right OPERADOR_POTENCIA
%precedence LOGICA_NOT
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
    COMANDO_RETURN Expressao
    | COMANDO_RETURN LITERAL
    | COMANDO_RETURN
    ;

ComandoSe:
    COMANDO_IF SIMBOLO_ABRE_PARENTESES Expressao SIMBOLO_FECHA_PARENTESES Bloco
    | COMANDO_IF SIMBOLO_ABRE_PARENTESES Expressao SIMBOLO_FECHA_PARENTESES Bloco COMANDO_ELSE Bloco
    ;

ComandoEnquanto:
    COMANDO_WHILE SIMBOLO_ABRE_PARENTESES Expressao SIMBOLO_FECHA_PARENTESES Bloco
    ;

ComandoAtribuicao:
    ID SIMBOLO_ATRIBUICAO Expressao
    | ID SIMBOLO_ATRIBUICAO LITERAL
    ;

ComandoEscrita:
    COMANDO_PRINT SIMBOLO_ABRE_PARENTESES Expressao SIMBOLO_FECHA_PARENTESES
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
    ListaParametros SIMBOLO_VIRGULA Expressao
    | ListaParametros SIMBOLO_VIRGULA LITERAL
    | Expressao
    | LITERAL
    ;

// FIM CODIGO COM BASE NA GRAMATICA DO PROFESSOR

Expressao:
    CONSTANTE_INT
    | CONSTANTE_FLOAT
    | ID
    | Expressao OPERADOR_SOMA Expressao
    | Expressao OPERADOR_SUBTRACAO Expressao
    | Expressao OPERADOR_MULTIPLICACAO Expressao
    | Expressao OPERADOR_DIVISAO Expressao
    | Expressao OPERADOR_POTENCIA Expressao
    | Expressao LOGICA_LT Expressao
    | Expressao LOGICA_GT Expressao
    | Expressao LOGICA_LE Expressao
    | Expressao LOGICA_GE Expressao
    | Expressao LOGICA_AND Expressao
    | Expressao LOGICA_OR Expressao
    | Expressao LOGICA_EQ Expressao
    | Expressao LOGICA_NE Expressao
    | LOGICA_NOT Expressao
    | SIMBOLO_ABRE_PARENTESES Expressao SIMBOLO_FECHA_PARENTESES

%%

int yyerror(const char *str) {
    fprintf(stderr, "%s\n", str);
    return 0;
}