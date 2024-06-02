#include "astUtils.h"

AST criar_programa(vector(struct AST) funcoes, vector(struct AST) blocoPrincipal) {
  AST programa;
  programa.tipo = Vetor;
  programa.token.tipo = Program;
  programa.token.u.nada = NULL;
  programa.u.filhos = concat_vectors(funcoes, blocoPrincipal);

  return programa;
}

vector(AST) concat_vectors(vector(AST) a, vector(AST) b) {
  vector(AST) result = NULL;

  if (a) {
    for (AST *it = cvector_begin(a); it != cvector_end(a); ++it) {
      cvector_push_back(result, *it);
    }
  }

  if (b) {
    for (AST *it = cvector_begin(b); it != cvector_end(b); ++it) {
      cvector_push_back(result, *it);
    }
  }

  return result;
}

AST criar_lista_funcoes(AST funcoes, AST funcao) {
  AST listaFuncoes;
  listaFuncoes.tipo = Vetor;
  listaFuncoes.token.tipo = DeclarationFunction;
  listaFuncoes.u.filhos = funcoes.u.filhos;

  cvector_push_back(listaFuncoes.u.filhos, funcoes);

  return listaFuncoes;
}