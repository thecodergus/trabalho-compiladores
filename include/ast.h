#ifndef DE9912FB_2382_4387_8C42_BD3C683D6A32
#define DE9912FB_2382_4387_8C42_BD3C683D6A32

#include <stdio.h>
#include <stdlib.h>
#include <utils/cvector.h>

#define vector_t(T) cvector_vector_type(T)

typedef struct Programa {
  vector_t(struct AST *) declacaoFuncoes;
  vector_t(struct AST *) blocoPrincipal;
} Programa;

typedef struct AST AST;

typedef enum Tipo {
  ProgramaPrincipal,
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

enum OperadoresAritmeticos {
  Soma,
  Subtracao,
  Multiplicacao,
  Divisao,
  Potenciacao,
};

enum OperadoresRelacionais {
  Igual,
  Diferente,
  Maior,
  MaiorIgual,
  Menor,
  MenorIgual,
};

enum OperadoresLogicos {
  E,
  Ou,
  Nao,
};

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
  enum OperadoresAritmeticos operador;
  AST *esquerda;
  AST *direita;
} ExpressaoAritmetica;

typedef struct ExpressaoRelacional {
  enum OperadoresRelacionais operador;
  AST *esquerda;
  AST *direita;
} ExpressaoRelacional;

typedef struct ExpressaoLogica {
  enum OperadoresLogicos operador;
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
  } token;
};

typedef struct token {
  int tipo;
  union {
    int inteiro;
    float flutuante;
    char *string, *id;
    AST *ast;
    cvector_vector_type(AST *) blocoComandos;
  } valor;
} Token;

struct Programa *criarPrograma(vector_t(AST *) declacaoFuncoes, vector_t(AST *) blocoPrincipal);

AST *criarAST(Tipo tipo);

AST *criarASTPrograma(AST *bloco);

AST *criarASTComandoIf(AST *condicao, AST *comando);

AST *criarASTComandoElse(AST *comando);

AST *criarASTComandoWhile(AST *condicao, AST *comando);

AST *criarASTComandoAtribuicao(char *id, AST *expressao);

AST *criarASTComandoPrint(AST *expressao);

AST *criarASTComandoRead(char *id);

AST *criarASTComandoReturn(AST *expressao);

AST *criarASTComandoChamadaFuncao(char *id, vector_t(AST *) * parametros);

AST *criarASTExpressaoAritmetica(enum OperadoresAritmeticos operador, AST *esquerda, AST *direita);

AST *criarASTExpressaoRelacional(enum OperadoresRelacionais operador, AST *esquerda, AST *direita);

AST *criarASTExpressaoLogica(enum OperadoresLogicos operador, AST *esquerda, AST *direita);

AST *criarASTExpressaoValorInteiro(int valor);

AST *criarASTExpressaoValorFloat(float valor);

AST *criarASTExpressaoValorString(char *valor);

#endif