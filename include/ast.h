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
  ExpressaoAritmeticaConvertida,
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
  AST *codicao;
  vector(AST *) comandosIf;
  vector(AST *) comandosElse;
};
struct While {
  AST *codicao;
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

struct ConverterExpressao {
  enum TipoDado de, para;
  AST *expr;
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
    struct ConverterExpressao aritmetica_convertida;
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
AST *criar_if(AST *expr, AST *comandosIf, AST *comandosElse);
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
void avaliar_main(AST *nodo);
void avaliar_bloco(const char *contexto, AST *bloco);
void avaliar_comando(const char *contexto, AST *comando);
void avaliar_atribuição(const char *contexto, AST *atribuicao);
void avaliar_chamada_funcao(const char *contexto, AST *chamada);
void avaliar_existencia_retorno_funcao(const char *id, AST *bloco);
void avaliar_retorno(const char *id, AST *retorno);
void avaliar_if(const char *contexto, AST *if_);
void avaliar_expressao_logica(const char *contexto, AST *expr);
void avaliar_expressao_relacional(const char *contexto, AST *expr);
void avaliar_while(const char *contexto, AST *comando);
void avaliar_print(const char *contexto, AST *print);
void avaliar_read(const char *contexto, AST *read);
void avaliar_expressao_aritmetica(const char *contexto, AST *expr);

// Extra
void reescrever_codigo(AST *programa);

// Gerar bytecode
void criar_arquivo_class(const char *nome, AST *programa);
void gerar_bytecode(FILE *arquivo, AST *programa, const char *contexto);
extern int LABEL_NUM;

// Utils
void percorrer(AST *a, void (*fn)(AST *));
void exibir_arvore(AST *no, int profundidade);
const char *tipoToken_para_str(enum TipoToken token);
char *repeat_char(int c, int n);
void constantInt_para_constantFloat(AST *constant);
void constantFloat_para_constantInt(AST *constant);
void expressaoAritmetica_para_Float(const char *contexto, AST *expr);
void expressaoAritmetica_para_Int(const char *contexto, AST *expr);
enum TipoDado descobrir_tipo_expressao(AST *expr);
enum TipoDado descobrir_tipo_expressao_com_contexto(const char *contexto, AST *expr);
void replace_char(char *str, char find, char replace);

#endif