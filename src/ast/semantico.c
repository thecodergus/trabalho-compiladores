#include "ast.h"

void avaliar_programa(AST *nodo) {
  //   if (nodo) {
  //     percorrer(nodo, lambda(void, (AST * no) { return; }));
  //   }
}

void avaliar_funcao(AST *nodo) {
  enum TipoDado tipo = nodo->funcao.retorno;
  const char *id = nodo->funcao.id;
  vector(struct Par) parametros = NULL;

  if (id_sendo_usado_por_funcao(id)) {
    exibir_erro(sprintf("O id '%s' ja esta sendo usado por uma funcao", id));
    return;
  }
}

void avaliar_main(AST *nodo) {}