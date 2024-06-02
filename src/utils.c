#include "utils.h"

void imprimir_arvore(AST* raiz, int nivel) {
  if (raiz == NULL) return;

  for (int i = 0; i < nivel; i++) printf("-");

  switch (raiz->tipo) {
    case Arvore:
      printf("Arvore:\n");
      imprimir_arvore(raiz->u.arvore.left, nivel + 1);
      imprimir_arvore(raiz->u.arvore.right, nivel + 1);
      break;
    case Vetor:
      printf("Vetor:\n");
      for (AST** i = cvector_begin(raiz->u.filhos); i != cvector_end(raiz->u.filhos); i++) {
        imprimir_arvore(*i, nivel + 1);
      }
      break;
    case Folha:
      printf("Folha: ");
      switch (raiz->token.tipo) {
        case Program:
          printf("Program\n");
          break;
        case DeclarationFunction:
          printf("DeclarationFunction, Type: %d\n", raiz->token.u.funcao.tipo);
          break;
        case DeclarationParameter:
          printf("DeclarationParameter, Type: %d\n", raiz->token.u.parametro.tipo);
          break;
        case DeclarationVariable:
          printf("DeclarationVariable, Type: %d\n", raiz->token.u.type.tipo);
          break;
        case ConstantInt:
          printf("ConstantInt: %d\n", raiz->token.u.constInt.valor);
          break;
        case ConstantFloat:
          printf("ConstantFloat: %f\n", raiz->token.u.constFloat.valor);
          break;
        case ConstantString:
          printf("ConstantString: %s\n", str_ptr(raiz->token.u.constString.valor));
          break;
        // Add cases for other tokens as needed
        default:
          printf("Unknown\n");
      }
      break;
  }
}