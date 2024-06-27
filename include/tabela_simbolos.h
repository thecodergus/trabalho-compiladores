#ifndef TABELA_SIMBOLOS_H
#define TABELA_SIMBOLOS_H

#include "libraries/cvector.h"
#include "tipos.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

enum TipoItem { Fn, Var };

typedef struct Linha {
  enum TipoItem tipo_item;
  char *escopo;
  char *id;
  enum TipoDado tipo_declarado;
} Linha;

extern cvector_vector_type(struct Linha) tabela_simbolos;

void adicionar_funcao(enum TipoDado tipo, const char *id);
void adicionar_variavel(const char *fn_id, enum TipoDado tipo, const char *id);
bool id_sendo_usado_por_variavel(const char *fn_id, const char *id);
bool id_sendo_usado_por_funcao(const char *id);
enum TipoDado get_tipo_dado_id_variavel(const char *fn_id, const char *id);
enum TipoDado get_tipo_dado_id_funcao(const char *id);


#endif