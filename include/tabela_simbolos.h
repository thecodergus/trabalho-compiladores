#ifndef TABELA_SIMBOLOS_H
#define TABELA_SIMBOLOS_H

#include "libraries/cvector.h"
#include "tipos.h"
#include <stdbool.h>
#include <stdlib.h>

enum TipoItem { Fn, Var };

typedef struct {
  enum TipoItem tipo_item;
  const char *escopo;
  const char *id;
  enum TipoDado tipo_declarado;
} Linha;

extern cvector_vector_type(Linha) tabela_simbolos;

#endif