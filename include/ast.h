#ifndef DE9912FB_2382_4387_8C42_BD3C683D6A32
#define DE9912FB_2382_4387_8C42_BD3C683D6A32

#include <stdio.h>
#include <stdlib.h>
#include <utils/cvector.h>

#define vector(T) cvector_vector_type(T)

// Criação dos tipos de dados
enum TipoNo {
  Program,
  DeclarationFunction,
  Block,
  Declaration,
  Assignment,
  If,
  Else,
  While,
  Return,
  Print,
  Read,
  CallFuncion,
  ExpressionArithmetic,
  ExpressionRelational,
  ExpressionLogical,
  Variable,
  Number,
  String,
  Void
};

// Criação dos tokens
typedef struct Token Token;

struct Token {
  enum TipoNo tipo;
};

// Criação do AST
enum TipoDado { Arvore, Vetor };

typedef struct AST AST;

struct AST {
  enum TipoDado tipo;
  Token token;
  union {
    struct {
      struct AST *left;
      struct AST *right;
    } arvore;
    vector(struct AST) filhos;
  } u;
};

// Definição das funções de criação de AST
AST criar_programa(vector(struct AST) funcoes, vector(struct AST) blocoPrincipal);

vector(AST) concat_vectors(vector(AST) a, vector(AST) b);

AST criar_lista_funcoes(AST funcoes, AST funcao);

#endif