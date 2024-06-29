%language "c"

%code requires{
    #include "ast.h"
}

%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "ast.h"


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
        avaliar_programa($1);
        exibir_arvore($1, 0);
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
        $$ = transicao($1, $2);
    }
    | Funcao{
        $$ = transicao(NULL, $1);
    }
    ;

Funcao:
    TipoRetorno ID SIMBOLO_ABRE_PARENTESES DeclaracaoParametros SIMBOLO_FECHA_PARENTESES BloboPrincipal{
        $$ = criar_funcao($1, $2, $4, $6);
    }
    | TipoRetorno ID SIMBOLO_ABRE_PARENTESES SIMBOLO_FECHA_PARENTESES BloboPrincipal{
        $$ = criar_funcao($1, $2, NULL, $5);
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
        $$ = transicao($1, $3);
    }
    | Parametro{
        $$ = transicao(NULL, $1);
    }
    ;

Parametro:
    Tipo ID{
        $$ = criar_parametro($1, $2);
    }

BloboPrincipal:
    SIMBOLO_ABRE_CHAVES Declaracoes ListaComando SIMBOLO_FECHA_CHAVES{
        $$ = criar_bloco($2, $3);
    }
    | SIMBOLO_ABRE_CHAVES ListaComando SIMBOLO_FECHA_CHAVES{
        $$ = criar_bloco(NULL, $2);
    }
    ;

Declaracoes:
    Declaracoes Declaracao{
        $$ = transicao($1, $2);
    }
    | Declaracao{
        $$ = transicao(NULL, $1);
    }
    ;

Declaracao:
    Tipo ListaId SIMBOLO_PONTO_VIRGULA{
        $$ = criar_variavel($1, $2);
    }
    ;

Tipo: 
    TIPO_STRING{
        $$ = criar_tipo(String);
    }
    | TIPO_INT{
        $$ = criar_tipo(Int);
    }
    | TIPO_FLOAT{
        $$ = criar_tipo(Float);
    }
    ;

ListaId:
    ListaId SIMBOLO_VIRGULA ID{
        $$ = transicao($1, $3);
    }
    | ID{
        $$ = transicao(NULL, $1);
    }
    ;

Bloco: 
    SIMBOLO_ABRE_CHAVES ListaComando SIMBOLO_FECHA_CHAVES{
        $$ = $2;
    }

ListaComando:
    ListaComando Comando{
        $$ = transicao($1, $2);
    }
    | Comando{
        $$ = transicao(NULL, $1);
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
        $$ = criar_retorno($2, descobrir_tipo_expressao($2));
    }
    | COMANDO_RETURN CONSTANTE_STRING{
        $$ = criar_retorno($2, String);
    }
    | COMANDO_RETURN{
        $$ = criar_retorno(NULL, Void);
    }
    ;

ComandoSe:
    COMANDO_IF SIMBOLO_ABRE_PARENTESES ExpressaoLogica SIMBOLO_FECHA_PARENTESES Bloco{
        $$ = criar_if($3, $5, NULL);
    }
    | COMANDO_IF SIMBOLO_ABRE_PARENTESES ExpressaoLogica SIMBOLO_FECHA_PARENTESES Bloco COMANDO_ELSE Bloco{
        $$ = criar_if($3, $5, $7);
    }
    ;

ComandoEnquanto:
    COMANDO_WHILE SIMBOLO_ABRE_PARENTESES ExpressaoLogica SIMBOLO_FECHA_PARENTESES Bloco{
        $$ = criar_enquanto($3, $5);
    }
    ;

ComandoAtribuicao:
    ID SIMBOLO_ATRIBUICAO ExpressaoAritmetica{
        $$ = criar_atribuicao($1, $3);
    }
    | ID SIMBOLO_ATRIBUICAO CONSTANTE_STRING {
        $$ = criar_atribuicao($1, $3);
    }
    | ID SIMBOLO_ATRIBUICAO ID{
        $$ = criar_atribuicao($1, $3);
    }
    ;

ComandoEscrita:
    COMANDO_PRINT SIMBOLO_ABRE_PARENTESES ExpressaoAritmetica SIMBOLO_FECHA_PARENTESES{
        $$ = criar_print($3);
    }
    | COMANDO_PRINT SIMBOLO_ABRE_PARENTESES CONSTANTE_STRING SIMBOLO_FECHA_PARENTESES{
        $$ = criar_print($3);
    }
    ;

ComandoLeitura:
    COMANDO_READ SIMBOLO_ABRE_PARENTESES ID SIMBOLO_FECHA_PARENTESES{
        $$ = criar_read($3);
    }
    ;

ChamadaProc:
    ChamadaFuncao{
        $$ = $1;
    }
    ;

ChamadaFuncao:
    ID SIMBOLO_ABRE_PARENTESES ListaParametros SIMBOLO_FECHA_PARENTESES{
        $$ = criar_chamada_funcao($1, $3);
    }
    | ID SIMBOLO_ABRE_PARENTESES SIMBOLO_FECHA_PARENTESES{
        $$ = criar_chamada_funcao($1, NULL);
    }
    ;

ListaParametros:
    ListaParametros SIMBOLO_VIRGULA ExpressaoAritmetica{
        $$ = transicao($1, $3);
    }
    | ListaParametros SIMBOLO_VIRGULA CONSTANTE_STRING{
        $$ = transicao($1, $3);
    }
    | ExpressaoAritmetica{
        $$ = transicao(NULL, $1);
    }
    | CONSTANTE_STRING{
        $$ = transicao(NULL, $1);
    }
    | ID{
        $$ = transicao(NULL, $1);
    }
    ;

// FIM CODIGO COM BASE NA GRAMATICA DO PROFESSOR


ExpressaoLogica:
    ExpressaoLogica LOGICA_AND TermoLogico{
        $$ = criar_operacao_logica($1, $2, "&&");
    }
    | ExpressaoLogica LOGICA_OR TermoLogico{
        $$ = criar_operacao_logica($1, $2, "||");
    }
    | TermoLogico{
        $$ = $1;
    }
    ;

TermoLogico:
    LOGICA_NOT TermoLogico{
        $$ = criar_operacao_logica($2, NULL, "!");
    }
    | ExpressaoRelacional{
        $$ = $1;
    }
    | SIMBOLO_ABRE_PARENTESES ExpressaoLogica SIMBOLO_FECHA_PARENTESES{
        $$ = $2;
    }
    ;

ExpressaoRelacional:
    CONSTANTE_STRING LOGICA_EQ CONSTANTE_STRING{
        $$ = criar_operacao_relacional($1, $2, "==");
        
    }
    | CONSTANTE_STRING LOGICA_EQ ExpressaoAritmetica{
        exibir_erro("Strings nao se misturam com expressoes aritmeticas!");
        $$ = NULL;        
    }
    | ExpressaoAritmetica LOGICA_EQ ExpressaoAritmetica{
        $$ = criar_operacao_relacional($1, $2, "==");
    }
    | ExpressaoAritmetica LOGICA_NE ExpressaoAritmetica{
        $$ = criar_operacao_relacional($1, $2, "!=");
    }
    | ExpressaoAritmetica LOGICA_LE ExpressaoAritmetica{
        $$ = criar_operacao_relacional($1, $2, "<=");
    }
    | ExpressaoAritmetica LOGICA_GE ExpressaoAritmetica{
        $$ = criar_operacao_relacional($1, $2, ">=");
    }
    | ExpressaoAritmetica LOGICA_LT ExpressaoAritmetica{
        $$ = criar_operacao_relacional($1, $2, "<");
    }
    | ExpressaoAritmetica LOGICA_GT ExpressaoAritmetica{
        $$ = criar_operacao_relacional($1, $2, ">");
    }
    ;

ExpressaoAritmetica:
    CONSTANTE_INT 
    | CONSTANTE_FLOAT
    | CONSTANTE_STRING
    | ID
    | ChamadaFuncao
    | ExpressaoAritmetica OPERADOR_SOMA ExpressaoAritmetica{
        $$ = criar_operacao_aritmetica($1, $3, "+");
    }
    | ExpressaoAritmetica OPERADOR_SUBTRACAO ExpressaoAritmetica{
        $$ = criar_operacao_aritmetica($1, $3, "-");
    }
    | ExpressaoAritmetica OPERADOR_MULTIPLICACAO ExpressaoAritmetica{
        $$ = criar_operacao_aritmetica($1, $3, "*");
    }
    | ExpressaoAritmetica OPERADOR_DIVISAO ExpressaoAritmetica{
        $$ = criar_operacao_aritmetica($1, $3, "/");
    }
    | ExpressaoAritmetica OPERADOR_POTENCIA ExpressaoAritmetica{
        $$ = criar_operacao_aritmetica($1, $3, "^");
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