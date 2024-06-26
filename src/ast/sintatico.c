#include "ast.h"

AST *criar_ast(const enum TipoToken tipo) {
  AST *aux = (AST *)malloc(sizeof(AST));
  aux->tipo = tipo;

  return aux;
}

AST *criar_int(const int i) {
  AST *aux = criar_ast(ConsanteInt);
  aux->inteiro = i;

  return aux;
}

AST *criar_float(const float f) {
  AST *aux = criar_ast(ConsanteFloat);
  aux->flutuante = f;

  return aux;
}

AST *criar_string(const char *string) {
  AST *aux = criar_ast(ConsanteString);
  aux->string = string;

  return aux;
}

AST *criar_id(const char *id) {
  AST *aux = criar_ast(Id);
  aux->id = id;

  return aux;
}

AST *transicao(AST *a, AST *b) {
  if (a && a->tipo == SituacaoTransicao) {
    cvector_push_back(a->transicao, b);

    return a;
  } else if (!a) {
    a = criar_ast(SituacaoTransicao);
    a->tipo = SituacaoTransicao;
    cvector_push_back(a->transicao, b);

    return a;
  } else {
    exibir_erro("Funcao 'transicao' sendo usada de forma errada");
  }

  return NULL;
}