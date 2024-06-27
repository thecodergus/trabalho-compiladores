#include "tabela_simbolos.h"

cvector_vector_type(struct Linha) tabela_simbolos = NULL;

void adicionar_funcao(enum TipoDado tipo, const char *id) {
  Linha aux;
  aux.escopo = NULL;
  aux.id = strdup(id);
  aux.tipo_item = Fn;
  aux.tipo_declarado = tipo;

  cvector_push_back(tabela_simbolos, aux);
}

void adicionar_variavel(const char *fn_id, enum TipoDado tipo, const char *id) {
  Linha aux;
  aux.escopo = strdup(fn_id);
  aux.tipo_declarado = tipo;
  aux.id = strdup(id);

  cvector_push_back(tabela_simbolos, aux);
}

bool id_sendo_usado_por_variavel(const char *fn_id, const char *id) {
  for (Linha *l = cvector_begin(tabela_simbolos);
       l != cvector_end(tabela_simbolos); l++) {
    if (strcmp(l->id, id) == 0 && l->tipo_item == Var &&
        strcmp(l->escopo, fn_id) == 0) {
      return true;
    }
  }

  return false;
}

bool id_sendo_usado_por_funcao(const char *id) {
  for (Linha *l = cvector_begin(tabela_simbolos);
       l != cvector_end(tabela_simbolos); l++) {
    if (strcmp(l->id, id) == 0 && l->tipo_item == Fn) {
      return true;
    }
  }

  return false;
}

enum TipoDado get_tipo_dado_id_variavel(const char *fn_id, const char *id) {
  for (Linha *l = cvector_begin(tabela_simbolos);
       l != cvector_end(tabela_simbolos); l++) {
    if (strcmps(l->id, id) == 0 && l->tipo_item == Var &&
        strcmp(l->escopo, fn_id) == 0) {
      return l->tipo_declarado;
    }
  }

  return SemTipo;
}

enum TipoDado get_tipo_dado_id_funcao(const char *id) {
  for (Linha *l = cvector_begin(tabela_simbolos);
       l != cvector_end(tabela_simbolos); l++) {
    if (strcmp(l->id, id) == 0 && l->tipo_item == Fn) {
      return l->tipo_declarado;
    }
  }

  return SemTipo;
}