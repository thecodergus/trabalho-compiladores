#ifndef DE9912FB_2382_4387_8C42_BD3C683D6A32
#define DE9912FB_2382_4387_8C42_BD3C683D6A32

#include <stdio.h>
#include <stdlib.h>
#include <utils/cvector.h>

typedef enum Tipo {
  BlocoCom,
  ComIf,
  ComElse,
  ComWhile,
  ComAtribuicao,
  ComPrint,
  ComRead,
  ComReturn,
  ComChamadaFuncao,
  ExprAritmetica,
  ExprRelacional,
  ExprLogica,
  ExprValorInteiro,
  ExprValorFloat,
  ExprValorString,
  TipInteiro,
  TipFloat,
  TipString,
  TipIdentificador,
} Tipo;

typedef struct AST AST;

typedef struct ComandoIf {
  AST *condicao;
  AST *comando;
} ComandoIf;

typedef struct ComandoElse {
  AST *comando;
} ComandoElse;

typedef struct ComandoWhile {
  AST *condicao;
  AST *comando;
} ComandoWhile;

typedef struct ComandoAtribuicao {
  char *id;
  AST *expressao;
} ComandoAtribuicao;

typedef struct ComandoPrint {
  AST *expressao;
} ComandoPrint;

typedef struct ComandoRead {
  char *id;
} ComandoRead;

typedef struct ComandoReturn {
  AST *expressao;
} ComandoReturn;

typedef struct ComandoChamadaFuncao {
  char *id;
  struct cvector *parametros;
} ComandoChamadaFuncao;

typedef struct ExpressaoAritmetica {
  char *operador;
  AST *esquerda;
  AST *direita;
} ExpressaoAritmetica;

typedef struct ExpressaoRelacional {
  char *operador;
  AST *esquerda;
  AST *direita;
} ExpressaoRelacional;

typedef struct ExpressaoLogica {
  char *operador;
  AST *esquerda;
  AST *direita;
} ExpressaoLogica;

typedef struct ExpressaoValorInteiro {
  int valor;
} ExpressaoValorInteiro;

typedef struct ExpressaoValorFloat {
  float valor;
} ExpressaoValorFloat;

typedef struct ExpressaoValorString {
  char *valor;
} ExpressaoValorString;

struct AST {
  Tipo tipo;
  union {
    ComandoIf comandoIf;
    ComandoElse comandoElse;
    ComandoWhile comandoWhile;
    ComandoAtribuicao comandoAtribuicao;
    ComandoPrint comandoPrint;
    ComandoRead comandoRead;
    ComandoReturn comandoReturn;
    ComandoChamadaFuncao comandoChamadaFuncao;
    ExpressaoAritmetica expressaoAritmetica;
    ExpressaoRelacional expressaoRelacional;
    ExpressaoLogica expressaoLogica;
    ExpressaoValorInteiro expressaoValorInteiro;
    ExpressaoValorFloat expressaoValorFloat;
    ExpressaoValorString expressaoValorString;
  } valor;
};

#endif