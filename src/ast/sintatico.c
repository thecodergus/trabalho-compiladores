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