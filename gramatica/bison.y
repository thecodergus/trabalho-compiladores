%language "c"

%code requires{
#include "ast.h"
#include "utils.h"
#include "libraries/cvector.h"
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
%define api.value.type {AST*}

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



%%
// COMEÇO CODIGO COM BASE NA GRAMATICA DO PROFESSOR

InicioPrograma:
    Programa FIM{
        $$ = $1;
        imprimir_arvore($1, 0);
    }
    | FIM
    ;

Programa:
    ListaFuncoes BloboPrincipal{
        $$ = criar_programa($1, $2);
    }
    | BloboPrincipal{
        $$ = criar_programa(NULL, $1);
    }
    ;

ListaFuncoes:
    ListaFuncoes Funcao{
        $$ = criar_lista_funcoes($1, $2);
    }
    | Funcao{
        $$ = criar_lista_funcoes(NULL, $1);
    }
    ;

Funcao:
    TipoRetorno ID SIMBOLO_ABRE_PARENTESES DeclaracaoParametros SIMBOLO_FECHA_PARENTESES BloboPrincipal{
        $$ = criar_funcao($1, $2, $4, $6);
    }
    | TipoRetorno ID SIMBOLO_ABRE_PARENTESES SIMBOLO_FECHA_PARENTESES BloboPrincipal{
        $$ = criar_funcao_input_void($1, $2, $5);
    }
    ;

TipoRetorno:
    Tipo{
        $$ = $1;
    }
    | TIPO_VOID{
        $$ = criar_tipo(Void);
    }
    ;

DeclaracaoParametros:
    DeclaracaoParametros SIMBOLO_VIRGULA Parametro{
        $$ = criar_declaracao_parametros($1, $3);
        
    }
    | Parametro{
        $$ = criar_declaracao_parametros(NULL, $1);
    }
    ;

Parametro:
    Tipo ID{
        $$ = criar_parametro($1, $2);
    }

BloboPrincipal:
    SIMBOLO_ABRE_CHAVES Declaracoes ListaComando SIMBOLO_FECHA_CHAVES{
        $$ = criar_bloco_principal($2, $3);
    }
    | SIMBOLO_ABRE_CHAVES ListaComando SIMBOLO_FECHA_CHAVES{
        $$ = criar_bloco_principal(NULL, $2);
    }
    ;

Declaracoes:
    Declaracoes Declaracao{
        $$ = criar_declaracoes($1, $2);
    }
    | Declaracao SIMBOLO_PONTO_VIRGULA{
        $$ = criar_declaracoes(NULL, $1);
    }
    ;

Declaracao:
    Tipo ListaId{
        $$ = criar_declaracao($1, $2);
    }
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
    ListaId SIMBOLO_VIRGULA ID{
        $$ = criar_lista_identificadores($1, $3);
    }
    | ID{
        $$ = criar_lista_identificadores(NULL, $1);
    }
    ;

Bloco: 
    SIMBOLO_ABRE_CHAVES ListaComando SIMBOLO_FECHA_CHAVES{
        $$ = $2;
    }

ListaComando:
    ListaComando Comando{
        $$ = criar_lista_comandos($1, $2);
    }
    | Comando{
        $$ = criar_lista_comandos(NULL, $1);
    }
    ;

Comando:
    ComandoSe{
        $$ = $1;
    }
    | ComandoEnquanto{
        $$ = $1;
    }
    | ComandoAtribuicao SIMBOLO_PONTO_VIRGULA{
        $$ = $1;
    }
    | ComandoEscrita SIMBOLO_PONTO_VIRGULA{
        $$ = $1;
    }
    | ComandoLeitura SIMBOLO_PONTO_VIRGULA{
        $$ = $1;
    }
    | ChamadaProc SIMBOLO_PONTO_VIRGULA{
        $$ = $1;
    }
    | Retorno SIMBOLO_PONTO_VIRGULA{
        $$ = $1;
    }
    ;

Retorno:
    COMANDO_RETURN ExpressaoAritmetica{
        $$ = $2;
    }
    | COMANDO_RETURN CONSTANTE_STRING{
        $$ = $2;
    }
    | COMANDO_RETURN{
        $$ = criar_constante_void();
    }
    ;

ComandoSe:
    COMANDO_IF SIMBOLO_ABRE_PARENTESES ExpressaoLogica SIMBOLO_FECHA_PARENTESES Bloco{
        $$ = criar_comando_if($3, $5, NULL);
    }
    | COMANDO_IF SIMBOLO_ABRE_PARENTESES ExpressaoLogica SIMBOLO_FECHA_PARENTESES Bloco COMANDO_ELSE Bloco{
        $$ = criar_comando_if($3, $5, $7);
    }
    ;

ComandoEnquanto:
    COMANDO_WHILE SIMBOLO_ABRE_PARENTESES ExpressaoLogica SIMBOLO_FECHA_PARENTESES Bloco{
        $$ = criar_comando_while($3, $5);
    }
    ;

ComandoAtribuicao:
    ID SIMBOLO_ATRIBUICAO ExpressaoAritmetica{
        $$ = criar_comando_atribuicao($1, $3);
    }
    | ID SIMBOLO_ATRIBUICAO CONSTANTE_STRING{
        $$ = criar_comando_atribuicao($1, $3);
    }
    ;

ComandoEscrita:
    COMANDO_PRINT SIMBOLO_ABRE_PARENTESES ExpressaoAritmetica SIMBOLO_FECHA_PARENTESES{
        $$ = criar_comando_print($3);
    }
    | COMANDO_PRINT SIMBOLO_ABRE_PARENTESES CONSTANTE_STRING SIMBOLO_FECHA_PARENTESES{
        $$ = criar_comando_print($3);
    }
    ;

ComandoLeitura:
    COMANDO_READ SIMBOLO_ABRE_PARENTESES ID SIMBOLO_FECHA_PARENTESES{
        $$ = criar_comando_read($3); 
    }
    ;

ChamadaProc:
    ChamadaFuncao{
        $$ = $1;
    }
    ;

ChamadaFuncao:
    ID SIMBOLO_ABRE_PARENTESES ListaParametros SIMBOLO_FECHA_PARENTESES{
        $$ = criar_comando_chamada_funcao($1, $3);
    }
    | ID SIMBOLO_ABRE_PARENTESES SIMBOLO_FECHA_PARENTESES{
        $$ = criar_comando_chamada_funcao($1, criar_constante_void());
    }
    ;

ListaParametros:
    ListaParametros SIMBOLO_VIRGULA ExpressaoAritmetica{
        $$ = criar_lista_parametros($1, $3);
    }
    | ListaParametros SIMBOLO_VIRGULA CONSTANTE_STRING{
        $$ = criar_lista_parametros($1, $3);
    }
    | ExpressaoAritmetica{
        $$ = $1;
    }
    | CONSTANTE_STRING{
        $$ = $1;
    }
    | ID{
        $$ = $1;
    }
    ;

// FIM CODIGO COM BASE NA GRAMATICA DO PROFESSOR


ExpressaoLogica:
    ExpressaoLogica LOGICA_AND TermoLogico{
        $$ = criar_expressao_logica(E, $1, $3);
    }
    | ExpressaoLogica LOGICA_OR TermoLogico{
        $$ = criar_expressao_logica(Ou, $1, $3);
    }
    | TermoLogico
    ;

TermoLogico:
    LOGICA_NOT TermoLogico{
        $$ = criar_expressao_logica(Nao, $2, NULL);
    }
    | ExpressaoRelacional
    | SIMBOLO_ABRE_PARENTESES ExpressaoLogica SIMBOLO_FECHA_PARENTESES{
        $$ = $2;
    }
    ;

ExpressaoRelacional:
    ExpressaoAritmetica LOGICA_EQ ExpressaoAritmetica{
        $$ = criar_expressao_relacional(Igual, $1, $3);
    }
    | ExpressaoAritmetica LOGICA_NE ExpressaoAritmetica{
        $$ = criar_expressao_relacional(Diferente, $1, $3);
    }
    | ExpressaoAritmetica LOGICA_LE ExpressaoAritmetica{
        $$ = criar_expressao_relacional(MenorIgual, $1, $3);
    }
    | ExpressaoAritmetica LOGICA_GE ExpressaoAritmetica{
        $$ = criar_expressao_relacional(MaiorIgual, $1, $3);
    }
    | ExpressaoAritmetica LOGICA_LT ExpressaoAritmetica{
        $$ = criar_expressao_relacional(Menor, $1, $3);
    }
    | ExpressaoAritmetica LOGICA_GT ExpressaoAritmetica{
        $$ = criar_expressao_relacional(Maior, $1, $3);
    }
    ;

ExpressaoAritmetica:
    CONSTANTE_INT 
    | CONSTANTE_FLOAT
    | ID
    | ChamadaFuncao
    | ExpressaoAritmetica OPERADOR_SOMA ExpressaoAritmetica{
        $$ = criar_expressao_aritmetica(Soma, $1, $3);
    }
    | ExpressaoAritmetica OPERADOR_SUBTRACAO ExpressaoAritmetica{
        $$ = criar_expressao_aritmetica(Subtracao, $1, $3);
    }
    | ExpressaoAritmetica OPERADOR_MULTIPLICACAO ExpressaoAritmetica{
        $$ = criar_expressao_aritmetica(Multiplicacao, $1, $3);
    }
    | ExpressaoAritmetica OPERADOR_DIVISAO ExpressaoAritmetica{
        $$ = criar_expressao_aritmetica(Divisao, $1, $3);
    }
    | ExpressaoAritmetica OPERADOR_POTENCIA ExpressaoAritmetica{
        $$ = criar_expressao_aritmetica(Potencia, $1, $3);
    }
    | SIMBOLO_ABRE_PARENTESES ExpressaoAritmetica SIMBOLO_FECHA_PARENTESES{
        $$ = $2;
    }
    ;
%%

int yyerror(const char *str) {
    fprintf(stderr, "%s\n", str);
    return 0;
}