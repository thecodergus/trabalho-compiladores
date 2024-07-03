#ifndef TABELA_SIMBOLOS_H
#define TABELA_SIMBOLOS_H

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "libraries/cvector.h"
#include "tipos.h"

enum TipoItem { Fn, Var };

struct Par {
  enum TipoDado tipo;
  const char *id;
};

typedef struct Linha {
  enum TipoItem tipo_item;
  char *escopo;
  char *id;
  enum TipoDado tipo_declarado;
  cvector_vector_type(struct Par) parametros;
  int posicao_pilha;
} Linha;

extern cvector_vector_type(struct Linha) tabela_simbolos;

void adicionar_funcao(enum TipoDado tipo, const char *id, cvector_vector_type(struct Par) parametros);
void adicionar_variavel(const char *fn_id, enum TipoDado tipo, const char *id);
bool id_sendo_usado_por_variavel(const char *fn_id, const char *id);
bool id_sendo_usado_por_funcao(const char *id);
enum TipoDado get_tipo_dado_variavel(const char *fn_id, const char *id);
enum TipoDado get_tipo_dado_funcao(const char *id);
cvector_vector_type(struct Par) get_parametros_funcao(const char *id);
int get_numero_posicoes_variavel_total(const char *contexto);
int get_numero_variavel_posicao(const char *contexto, const char *id);
int get_numero_posicoes_funcao_total();
int get_numero_funcao_posicao(const char *id);

#endif