%language "c"

%code top{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int yyerror(const char *);
int yylex();

%}

%define parse.error verbose

// Simbolos de comandos
%token  COMANDO_PRINT COMANDO_READ COMANDO_IF COMANDO_ELSE COMANDO_WHILE COMANDO_RETURN

// Simbolo de identificadores de variaveis e funcoes
%token ID LITERAL

// Simbbolos de tipos de variaveis e funcoes
%token TIPO_VOID TIPO_INT TIPO_FLOAT TIPO_STRING

// Simbolos de operadores aritmeticos
%token OPERADOR_MULTIPLICACAO OPERADOR_DIVISAO OPERADOR_SOMA OPERADOR_SUBTRACAO OPERADOR_POTENCIA 

// Simbolos de operadores de atribuicao
%token  SIMBOLO_ATRIBUICAO 

// Simbolos de abrir e fechar parenteses
%token SIMBOLO_ABRE_PARENTESES SIMBOLO_FECHA_PARENTESES

// Simbolos de abrir e fechar chaves
%token SIMBOLO_ABRE_CHAVES SIMBOLO_FECHA_CHAVES

// Simbolo de virgula e ponto e virgula
%token SIMBOLO_VIRGULA SIMBOLO_PONTO_VIRGULA

// Simbolo de fim de programa
%token FIM


// Operadores Logicos
%left LOGICA_OR
%left LOGICA_AND
%left LOGICA_EQ LOGICA_NE
%left LOGICA_LT LOGICA_LE LOGICA_GT LOGICA_GE
%precedence LOGICA_NOT

// Operadores Aritmeticos
%left OPERADOR_SOMA OPERADOR_SUBTRACAO
%left OPERADOR_MULTIPLICACAO OPERADOR_DIVISAO
%left OPERADOR_POTENCIA
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
    | Declaracao
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
    | Comando
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
    COMANDO_RETURN ExpressaoAritmetica SIMBOLO_PONTO_VIRGULA
    | COMANDO_RETURN LITERAL SIMBOLO_PONTO_VIRGULA
    | COMANDO_RETURN SIMBOLO_PONTO_VIRGULA
    ;

ComandoSe:
    COMANDO_IF SIMBOLO_ABRE_PARENTESES ExpressaoLogica SIMBOLO_FECHA_PARENTESES Bloco
    | COMANDO_IF SIMBOLO_ABRE_PARENTESES ExpressaoLogica SIMBOLO_FECHA_PARENTESES Bloco COMANDO_ELSE Bloco
    ;

ComandoEnquanto:
    COMANDO_WHILE SIMBOLO_ABRE_PARENTESES ExpressaoLogica SIMBOLO_FECHA_PARENTESES Bloco
    ;

ComandoAtribuicao:
    ID SIMBOLO_ATRIBUICAO ExpressaoAritmetica SIMBOLO_PONTO_VIRGULA
    | ID SIMBOLO_ATRIBUICAO LITERAL SIMBOLO_PONTO_VIRGULA
    ;

ComandoEscrita:
    COMANDO_PRINT SIMBOLO_ABRE_PARENTESES ExpressaoAritmetica SIMBOLO_FECHA_PARENTESES SIMBOLO_PONTO_VIRGULA
    | COMANDO_PRINT SIMBOLO_ABRE_PARENTESES LITERAL SIMBOLO_FECHA_PARENTESES SIMBOLO_PONTO_VIRGULA
    ;

ComandoLeitura:
    COMANDO_READ SIMBOLO_ABRE_PARENTESES ID SIMBOLO_FECHA_PARENTESES SIMBOLO_PONTO_VIRGULA
    ;

ComandoRetorno:
    ChamaFuncao SIMBOLO_PONTO_VIRGULA
    ;

ChamaFuncao:
    ID SIMBOLO_ABRE_PARENTESES ListaParametros SIMBOLO_FECHA_PARENTESES
    | ID SIMBOLO_ABRE_PARENTESES SIMBOLO_FECHA_PARENTESES
    ;

ListaParametros:
    ListaParametros SIMBOLO_VIRGULA ExpressaoAritmetica
    | ListaParametros SIMBOLO_VIRGULA LITERAL
    | ExpressaoAritmetica
    | LITERAL
    ;

ExpressaoAritmetica:
    ExpressaoAritmetica OPERADOR_SOMA ExpressaoAritmetica {$$ = $1 + $3;}
    | ExpressaoAritmetica OPERADOR_SUBTRACAO ExpressaoAritmetica {$$ = $1 - $3;}
    | Fator {$$ = $1;}
    ;

Fator:
    Termo {$$ = $1;}
    | Fator OPERADOR_MULTIPLICACAO Termo {$$ = $1 * $3;}
    | Fator OPERADOR_DIVISAO Termo {$$ = $1 / $3;}
    ;

Termo:
    ID {$$ = $1;}
    | LITERAL {$$ = $1;}
    

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