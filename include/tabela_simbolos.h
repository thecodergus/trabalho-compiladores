#ifndef TABELA_SIMBOLOS_H
#define TABELA_SIMBOLOS_H

#include "libraries/cvector.h"
#include "tipos.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

enum TipoItem { Fn, Var };

typedef struct {
  enum TipoItem tipo_item;
  char *escopo;
  char *id;
  enum TipoDado tipo_declarado;
} Linha;

extern cvector_vector_type(Linha) tabela_simbolos;

void adicionar_funcao(enum TipoDado tipo, const char *id);
void adicionar_variavel(const char *fn_id, enum TipoDado tipo, const char *id);


#endif