#include "ast.h"

void avaliar_programa(AST *nodo) {
  if (nodo && nodo->tipo == Programa) {
    for (AST **it = cvector_begin(nodo->programa.funcoes); it != cvector_end(nodo->programa.funcoes); it++) {
      avaliar_funcao(*it);
    }
  }
}

void avaliar_funcao(AST *nodo) {
  enum TipoDado tipo = nodo->funcao.retorno;
  const char *id = strdup(nodo->funcao.id);
  vector(struct Par) parametros = NULL;

  if (id_sendo_usado_por_funcao(id)) {
    char msg[1000];
    sprintf(msg, "O id '%s' ja esta sendo usado por uma funcao", id);
    exibir_erro(msg);
    return;
  } else {
    // Adicionando a função e os parametros na tabela de simbolos
    for (AST **it = cvector_begin(nodo->funcao.parametros); it != cvector_end(nodo->funcao.parametros); it++) {
      struct Par par;

      par.id = strdup((*it)->parametro.id);
      par.tipo = (*it)->parametro.tipo;

      cvector_push_back(parametros, par);

      // Adicionando parametro
      if (id_sendo_usado_por_funcao((*it)->parametro.id)) {
        char msg[1000];
        sprintf(msg, "O id '%s' do parametro da funcao '%s' ja eh usado por outra funcao", (*it)->parametro.id, id);
        exibir_erro(msg);
      } else if (id_sendo_usado_por_variavel(id, (*it)->parametro.id)) {
        char msg[1000];
        sprintf(msg, "O id '%s' ja esta sendo usado por uma variavel no mesmo escopo", (*it)->parametro.id);
        exibir_erro(msg);
      } else {
        adicionar_variavel(id, (*it)->parametro.tipo, (*it)->parametro.id);
      }
    }

    // Adicionando a tabela de simbolos
    adicionar_funcao(tipo, id, parametros);
  }
}

void avaliar_main(AST *nodo) {}