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
%left OPERADOR_SOMA OPERADOR_SUBTRACAO
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

Literal:
    LITERAL

Constante:
    CONSTANTE_INT
    | CONSTANTE_FLOAT
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
    COMANDO_RETURN ExpressaoAritimetica
    | COMANDO_RETURN Literal
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
    ID SIMBOLO_ATRIBUICAO ExpressaoAritimetica
    | ID SIMBOLO_ATRIBUICAO Literal
    ;

ComandoEscrita:
    COMANDO_PRINT SIMBOLO_ABRE_PARENTESES ExpressaoAritimetica SIMBOLO_FECHA_PARENTESES
    | COMANDO_PRINT SIMBOLO_ABRE_PARENTESES Literal SIMBOLO_FECHA_PARENTESES
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
    ListaParametros SIMBOLO_VIRGULA ExpressaoAritimetica
    | ListaParametros SIMBOLO_VIRGULA Literal
    | ExpressaoAritimetica
    | Literal
    ;

// FIM CODIGO COM BASE NA GRAMATICA DO PROFESSOR
ExpressaoLogica:
    ExpressaoLogica LOGICA_OR SubExpressaoLogica
    | LOGICA_NOT SubExpressaoLogica
    | SIMBOLO_ABRE_PARENTESES ExpressaoLogica SIMBOLO_FECHA_PARENTESES
    | SubExpressaoLogica


SubExpressaoLogica:
    SubExpressaoLogica LOGICA_AND ExpressaoRelacional
    | ExpressaoRelacional

// Espaço para as comparações
ExpressaoRelacional:
    ExpressaoRelacional LOGICA_EQ ExpressaoAritimetica
    | ExpressaoRelacional LOGICA_NE ExpressaoAritimetica
    | ExpressaoRelacional LOGICA_LE ExpressaoAritimetica
    | ExpressaoRelacional LOGICA_GE ExpressaoAritimetica
    | ExpressaoRelacional LOGICA_LT ExpressaoAritimetica
    | ExpressaoRelacional LOGICA_GT ExpressaoAritimetica
    | ExpressaoAritimetica

ExpressaoAritimetica:
    ExpressaoAritimetica OPERADOR_SOMA SubExpressaoAritimetica
    | ExpressaoAritimetica OPERADOR_SUBTRACAO SubExpressaoAritimetica
    | OPERADOR_SUBTRACAO ExpressaoAritimetica
    | SubExpressaoAritimetica

SubExpressaoAritimetica:
    SubExpressaoAritimetica OPERADOR_MULTIPLICACAO SubSubExpressaoAritimetica
    | SubExpressaoAritimetica OPERADOR_DIVISAO SubSubExpressaoAritimetica
    | ChamadaFuncao
    | SubSubExpressaoAritimetica

SubSubExpressaoAritimetica:
    SubSubExpressaoAritimetica OPERADOR_POTENCIA FatorAritmetico
    | FatorAritmetico

FatorAritmetico:
    ID
    | Constante

%%

int yyerror(const char *str) {
    fprintf(stderr, "%s\n", str);
    return 0;
}