#include "tabela_simbolos.h"

cvector_vector_type(Linha) tabela_simbolos = NULL;

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