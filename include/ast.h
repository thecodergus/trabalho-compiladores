#ifndef DE9912FB_2382_4387_8C42_BD3C683D6A32
#define DE9912FB_2382_4387_8C42_BD3C683D6A32

#include <stdio.h>
#include <stdlib.h>

#include "utils/cvector.h"
#include "utils/str.h"

#define vector(T) cvector_vector_type(T)

// Tipos de dados
enum TipoDados { Int, Float, String, Void };

// Criação dos tipos de dados
typedef struct TipoDado TipoDado;

enum TipoToken {
  Program,
  DeclarationFunction,
  Parameter,
  Parameters,
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
  Type,
  Literal,
  ConstantInt,
  ConstantFloat,
  ConstantString
};

// Estrutura de dados para os tokens
struct Funcao {
  str id;
  enum TipoDados tipo;
};

struct Parametro {
  str id;
  enum TipoDados tipo;
};

struct Type {
  enum TipoDados tipo;
};

struct ConstanteInt {
  int valor;
};

struct ConstanteFloat {
  float valor;
};

struct ConstanteString {
  str valor;
};

// Criação dos tokens
typedef struct Token Token;

struct Token {
  enum TipoToken tipo;
  union {
    struct Funcao funcao;
    struct Parametro parametro;
    struct Type type;
    void *nada;
    struct ConstanteInt constInt;
    struct ConstanteFloat constFloat;
    struct ConstanteString constString;
  } u;
};

// Criação do AST
enum TipoNo { Arvore, Vetor };

typedef struct AST AST;

struct AST {
  enum TipoNo tipo;
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

AST criar_funcao(AST parametros, AST bloco);

#endif