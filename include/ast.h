#ifndef DE9912FB_2382_4387_8C42_BD3C683D6A32
#define DE9912FB_2382_4387_8C42_BD3C683D6A32

#include <stdio.h>
#include <stdlib.h>

#include "libraries/cvector.h"
#include "libraries/lambda.h"

#define vector(T) cvector_vector_type(T)

enum TipoToken {
  Programa,
  Funcao,
  Parametro,
  Bloco,
  Atribuicao,
  If,
  While,
  Retorno,
  Print,
  Read,
  ChamadaFuncao,
  ExpressaoRelacional,
  ExpressaoLogica,
  ExpressaoAritmetica,
  ConsanteInt,
  ConsanteFloat,
  ConsanteString,
  Id,
  SituacaoTransicao,
  Desconhecido
};

enum TipoDado { Int, Float, String };
typedef struct AST AST;

struct ExpressaoRelacional {
  const char *simbolo;
  AST *esquerda, *direita;
};
struct ExpressaoLogica {
  const char *simbolo;
  AST *esquerda, *direita;
};
struct ExpressaoAritmetica {
  const char *simbolo;
  AST *esquerda, *direita;
};
struct Programa {
  vector(AST *) funcoes;
  vector(AST *) bloco;
};
struct Funcao {
  const char *id;
  vector(AST *) parametros;
  vector(AST *) bloco;
};
struct Parametro {
  enum TipoDado tipo;
  const char *id;
};
struct Atribuicao {
  const char *id;
  struct ExpressaoAritmetica expressao;
};
struct If {
  struct ExpressaoRelacional codicao;
  vector(AST *) blocoIf;
  vector(AST *) blocoElse;
};
struct While {
  struct ExpressaoRelacional codicao;
  vector(AST *) bloco;
};
struct Print {
  vector(AST *) parametros;
};
struct Read {
  vector(AST *) parametros;
};

struct AST {
  enum TipoToken tipo;
  union {
    int inteiro;
    float flutuante;
    char *string;
    char *id;
    vector(struct AST *) transicao;
    struct Programa programa;
    struct Funcao funcao;
    struct Parametro parametro;
    struct Atribuicao atribuicao;
    struct If if_;
    struct While while_;
    struct Print print;
    struct Read read;
    struct ExpressaoRelacional relacional;
    struct ExpressaoLogica logica;
    struct ExpressaoAritmetica aritmetica;
  };
};

AST *criar_ast(const enum TipoToken tipo);
AST *criar_int(const int i);
AST *criar_float(const float f);
AST *criar_string(const char *string);
AST *criar_id(const char *id);

#endif