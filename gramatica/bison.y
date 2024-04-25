%language "c"

%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int yyerror(const char *);
int yylex();

%}

%define parse.error verbose

// Simbolos de comandos
%token  COMANDO_PRINT COMANDO_READ COMANDO_IF COMANDO_ELSE COMANDO_WHILE COMANDO_RETURN

// Simbolo de identificadores de variaveis e funcoes
%token ID LITERAL

// Simbolo de tipos
%token TIPO_VOID TIPO_FLOAT TIPO_INT TIPO_STRING

// Simbolos de abrir e fechar chaves
%token SIMBOLO_ABRE_CHAVES SIMBOLO_FECHA_CHAVES

// Simbolocos de abre e fecha parenteses
%token SIMBOLO_ABRE_PARENTESES SIMBOLO_FECHA_PARENTESES

// Simbolo de virgula e ponto e virgula
%token SIMBOLO_VIRGULA SIMBOLO_PONTO_VIRGULA

// Simbolo de fim de programa
%token FIM


// Operadores
%right SIMBOLO_ATRIBUICAO
%right LOGICA_EQ LOGICA_GT LOGICA_LT LOGICA_GE LOGICA_LE LOGICA_NE
%left OPERADOR_SOMA OPERADOR_SUBTRACAO
%left OPERADOR_MULTIPLICACAO OPERADOR_DIVISAO
%right OPERADOR_POTENCIA

// Tokens não associativos
%nonassoc LOGICA_OR LOGICA_AND LOGICA_NOT
%nonassoc COMANDO_IF COMANDO_ELSE COMANDO_PRINT COMANDO_READ COMANDO_RETURN COMANDO_WHILE
%nonassoc SIMBOLO_ABRE_CHAVES SIMBOLO_FECHA_CHAVES SIMBOLO_ABRE_PARENTESES SIMBOLO_FECHA_PARENTESES SIMBOLO_VIRGULA SIMBOLO_PONTO_VIRGULA
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
    ;

ComandoEscrita:
    COMANDO_PRINT SIMBOLO_ABRE_PARENTESES ExpressaoAritmetica SIMBOLO_FECHA_PARENTESES SIMBOLO_PONTO_VIRGULA
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
    | ExpressaoAritmetica
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