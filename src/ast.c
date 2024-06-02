#include "ast.h"

AST criar_programa(vector(struct AST *) funcoes, vector(struct AST *) blocoPrincipal) {
  AST programa;
  programa.tipo = Vetor;
  programa.u.filhos = NULL;

  return programa;
}

vector(AST *) concat_vectors(vector(AST *) a, vector(AST *) b) {
  vector(AST *) result = NULL;

  for (AST *it = cvector_begin(a); it != cvector_end(a); ++it) {
    cvector_push_back(result, it);
  }

  for (AST *it = cvector_begin(b); it != cvector_end(b); ++it) {
    cvector_push_back(result, it);
  }

  return result;
}