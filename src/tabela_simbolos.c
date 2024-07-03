#include "tabela_simbolos.h"

cvector_vector_type(struct Linha) tabela_simbolos = NULL;

void adicionar_funcao(enum TipoDado tipo, const char *id, cvector_vector_type(struct Par) parametros) {
  Linha aux;
  aux.escopo = NULL;
  aux.id = strdup(id);
  aux.tipo_item = Fn;
  aux.tipo_declarado = tipo;
  aux.parametros = parametros;
  aux.posicao_pilha = get_numero_posicoes_funcao_total();

  cvector_push_back(tabela_simbolos, aux);
}

void adicionar_variavel(const char *fn_id, enum TipoDado tipo, const char *id) {
  Linha aux;
  aux.escopo = strdup(fn_id);
  aux.tipo_declarado = tipo;
  aux.tipo_item = Var;
  aux.id = strdup(id);
  aux.parametros = NULL;
  aux.posicao_pilha = get_numero_posicoes_variavel_total(fn_id);

  cvector_push_back(tabela_simbolos, aux);
}

bool id_sendo_usado_por_variavel(const char *fn_id, const char *id) {
  for (Linha *l = cvector_begin(tabela_simbolos); l != cvector_end(tabela_simbolos); l++) {
    if (strcmp(l->id, id) == 0 && l->tipo_item == Var && strcmp(l->escopo, fn_id) == 0) {
      return true;
    }
  }

  return false;
}

bool id_sendo_usado_por_funcao(const char *id) {
  for (Linha *l = cvector_begin(tabela_simbolos); l != cvector_end(tabela_simbolos); l++) {
    if (strcmp(l->id, id) == 0 && l->tipo_item == Fn) {
      return true;
    }
  }

  return false;
}

enum TipoDado get_tipo_dado_variavel(const char *fn_id, const char *id) {
  for (Linha *l = cvector_begin(tabela_simbolos); l != cvector_end(tabela_simbolos); l++) {
    if (strcmp(l->id, id) == 0 && l->tipo_item == Var && strcmp(l->escopo, fn_id) == 0) {
      return l->tipo_declarado;
    }
  }

  return SemTipo;
}

enum TipoDado get_tipo_dado_funcao(const char *id) {
  for (Linha *l = cvector_begin(tabela_simbolos); l != cvector_end(tabela_simbolos); l++) {
    if (strcmp(l->id, id) == 0 && l->tipo_item == Fn) {
      return l->tipo_declarado;
    }
  }

  return SemTipo;
}

cvector_vector_type(struct Par) get_parametros_funcao(const char *id) {
  for (Linha *l = cvector_begin(tabela_simbolos); l != cvector_end(tabela_simbolos); l++) {
    if (strcmp(l->id, id) == 0 && l->tipo_item == Fn) {
      return l->parametros;
    }
  }

  return NULL;
}

int get_numero_posicoes_variavel_total(const char *contexto) {
  int ultima_posicao = 0;

  for (Linha *l = cvector_begin(tabela_simbolos); l != cvector_end(tabela_simbolos); l++) {
    if (strcmp(l->id, contexto) == 0 && l->tipo_item == Var) {
      ultima_posicao++;
    }
  }

  return ultima_posicao;
}
int get_numero_variavel_posicao(const char *contexto, const char *id) {
  for (Linha *l = cvector_begin(tabela_simbolos); l != cvector_end(tabela_simbolos); l++) {
    if (strcmp(l->id, id) == 0 && l->tipo_item == Var && strcmp(l->escopo, contexto) == 0) {
      return l->posicao_pilha;
    }
  }
}
int get_numero_posicoes_funcao_total() {
  int ultima_posicao = 0;

  for (Linha *l = cvector_begin(tabela_simbolos); l != cvector_end(tabela_simbolos); l++) {
    if (l->tipo_item == Fn) {
      ultima_posicao++;
    }
  }

  return ultima_posicao;
}

int get_numero_funcao_posicao(const char *id) {
  for (Linha *l = cvector_begin(tabela_simbolos); l != cvector_end(tabela_simbolos); l++) {
    if (l->tipo_item == Fn && strcmp(l->id, id) == 0) {
      return l->posicao_pilha;
    }
  }

  return -1;
}