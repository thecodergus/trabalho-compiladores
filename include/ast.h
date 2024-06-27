#ifndef DE9912FB_2382_4387_8C42_BD3C683D6A32
#define DE9912FB_2382_4387_8C42_BD3C683D6A32

#include <stdio.h>
#include <stdlib.h>

#include "libraries/cvector.h"
#include "libraries/lambda.h"
#include "mensagens.h"
#include "tabela_simbolos.h"
#include "tipos.h"

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
  Tipo,
  ConsanteInt,
  ConsanteFloat,
  ConsanteString,
  Id,
  SituacaoTransicao,
  Variavel,
  Desconhecido
};

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

struct Bloco {
  vector(AST *) declaracoes;
  vector(AST *) comandos;
};

struct Programa {
  vector(AST *) funcoes;
  AST *bloco;
};
struct Funcao {
  enum TipoDado retorno;
  const char *id;
  vector(AST *) parametros;
  AST *bloco;
};
struct Parametro {
  enum TipoDado tipo;
  const char *id;
};
struct Atribuicao {
  const char *id;
  AST *expressao;
};
struct If {
  struct ExpressaoLogica codicao;
  vector(AST *) blocoIf;
  vector(AST *) blocoElse;
};
struct While {
  struct ExpressaoLogica codicao;
  vector(AST *) bloco;
};
struct Print {
  AST *parametro;
};
struct Read {
  const char *id;
};

struct ChamadaFuncao {
  const char *id;
  vector(AST *) parametros;
};

struct Retorno {
  enum TipoDado tipo;

  AST *ret;
};

struct Variavel {
  enum TipoDado tipo;
  vector(AST *) ids;
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
    struct ChamadaFuncao chamada_funcao;
    struct Retorno retorno;
    enum TipoDado declaracao_tipo;
    struct Variavel variavel;
    struct Bloco bloco;
  };
};

// Sintatico
AST *criar_ast(const enum TipoToken tipo);
AST *criar_int(const int i);
AST *criar_float(const float f);
AST *criar_string(const char *string);
AST *criar_id(const char *id);
AST *transicao(AST *a, AST *b);
AST *criar_operacao_aritmetica(AST *a, AST *b, const char *simbolo);
AST *criar_operacao_relacional(AST *a, AST *b, const char *simbolo);
AST *criar_operacao_logica(AST *a, AST *b, const char *simbolo);
AST *criar_chamada_funcao(AST *id, AST *parametros);
AST *criar_read(AST *id);
AST *criar_print(AST *instancia);
AST *criar_atribuicao(AST *id, AST *expr);
AST *criar_enquanto(AST *expr, AST *bloco);
AST *criar_if(AST *expr, AST *blocoIf, AST *blocoElse);
AST *criar_retorno(AST *ret, enum TipoDado tipo);
AST *criar_tipo(enum TipoDado tipo);
AST *criar_variavel(AST *tipo, AST *lista_ids);
AST *criar_bloco(AST *declaracoes, AST *comandos);
AST *criar_parametro(AST *tipo, AST *id);
AST *criar_funcao(AST *retorno, AST *id, AST *parametros, AST *bloco);
AST *criar_programa(AST *funcoes, AST *main);

// Semantico
void avaliar_programa(AST *nodo);
void avaliar_funcao(AST *nodo);

// Utils
void percorrer(AST *a, void (*fn)(AST *));
void exibir_arvore(AST *no);
const char *tipoToken_para_str(enum TipoToken token);
#endif