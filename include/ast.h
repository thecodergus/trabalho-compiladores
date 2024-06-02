#ifndef DE9912FB_2382_4387_8C42_BD3C683D6A32
#define DE9912FB_2382_4387_8C42_BD3C683D6A32

#include <stdio.h>
#include <stdlib.h>

#include "libraries/cvector.h"
#include "libraries/str.h"

#define vector(T) cvector_vector_type(T)

// Tipos de dados
enum TipoDados { Int, Float, String, Void };

// Criação dos tipos de dados
typedef struct TipoDado TipoDado;

enum TipoToken {
  Program,
  DeclarationFunction,
  Parameter,
  ParameterList,
  Block,
  DeclarationVariable,
  DeclarationList,
  Assignment,
  If,
  Else,
  While,
  Return,
  Print,
  Read,
  Function,
  FunctionList,
  CallFuncion,
  ExpressionArithmetic,
  ExpressionRelational,
  ExpressionLogical,
  Variable,
  Type,
  Literal,
  ConstantInt,
  ConstantFloat,
  ConstantString,
  ConstantVoid,
  Identifier,
  IdentifierList,
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

struct Indentificador {
  str id;
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
    struct Indentificador idenfier;
  } u;
};

// Criação do AST
enum TipoNo { Arvore, Vetor, Folha };

typedef struct AST AST;

struct AST {
  enum TipoNo tipo;
  Token token;
  union {
    struct {
      struct AST *left;
      struct AST *right;
    } arvore;
    vector(struct AST *) filhos;
  } u;
};

// Definição das funções de criação de AST
AST *criar_programa(AST *funcoes, AST *blocoPrincipal);

AST *criar_lista_funcoes(AST *funcoes, AST *funcao);

AST *criar_parametro(AST *tipo, AST *id);

AST *criar_declaracao_parametros(AST *declaracaoAnterior, AST *parametro);

AST *criar_funcao(AST *tipo, AST *id, AST *parametros, AST *bloco);

AST *criar_funcao_input_void(AST *tipo, AST *id, AST *bloco);

AST *criar_tipo(enum TipoDados tipo);

AST *criar_constante_int(const char *input);

AST *criar_constante_float(const char *input);

AST *criar_constante_string(const char *input);

AST *criar_constante_void();

AST *criar_idenfier(const char *input);

AST *criar_bloco_principal(AST *declaracoes, AST *comandos);

AST *criar_lista_identificadores(AST *identificadores, AST *id);

AST *criar_declaracao(AST *tipo, AST *ids);

AST *criar_declaracoes(AST *declaracoes, AST *declaracao);

#endif