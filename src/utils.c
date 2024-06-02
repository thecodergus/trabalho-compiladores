#include "utils.h"

void imprimir_arvore(AST* raiz, int nivel) {
  if (raiz == NULL) return;

  imprimir_traco(nivel);

  switch (raiz->tipo) {
    case Arvore:
      printf("Arvore:\n");
      imprimir_token(raiz->token, nivel + 1);
      imprimir_arvore(raiz->u.arvore.left, nivel + 2);
      imprimir_arvore(raiz->u.arvore.right, nivel + 2);
      break;
    case Vetor:
      printf("Vetor:\n");
      imprimir_token(raiz->token, nivel);
      for (AST** i = cvector_begin(raiz->u.filhos); i != cvector_end(raiz->u.filhos); i++) {
        imprimir_arvore(*i, nivel + 2);
      }
      break;
    case Folha:
      printf("Folha: \n");
      imprimir_token(raiz->token, nivel + 1);
      break;
  }
}

char* tipo_dado_str(enum TipoDados tipo) {
  switch (tipo) {
    case Int:
      return "Inteiro";
    case Float:
      return "Flutuante";
    case String:
      return "String";
    case Void:
      return "Void";
    default:
      return "Unknown";
  }
}

void printar_folhas(AST* arvore) {
  if (!arvore) return;

  switch (arvore->tipo) {
    case Folha:
      switch (arvore->token.tipo) {
        case ConstantInt:
          printf("ConstantInt: %d\n", arvore->token.u.constInt.valor);
          break;
        case ConstantFloat:
          printf("ConstantFloat: %f\n", arvore->token.u.constFloat.valor);
          break;
        case ConstantString:
          printf("ConstantString: %s\n", str_ptr(arvore->token.u.constString.valor));
          break;
        case Identifier:
          printf("Identifier: %s\n", str_ptr(arvore->token.u.idenfier.id));
          break;
        default:
          break;
      }
      break;
    case Arvore:
      if (arvore->u.arvore.left) printar_folhas(arvore->u.arvore.left);
      if (arvore->u.arvore.right) printar_folhas(arvore->u.arvore.right);
      break;
    case Vetor:
      if (arvore->u.filhos) {
        for (AST** i = cvector_begin(arvore->u.filhos); i != cvector_end(arvore->u.filhos); i++) {
          printar_folhas(*i);
        }
      }
      break;
  }
}

void imprimir_token(Token token, int nivel) {
  imprimir_traco(nivel);

  switch (token.tipo) {
    case Program:
      printf("Programa\n");
      break;
    case DeclarationFunction:
      // printf("Declaracao de funcao, ID: %s, Type: %s\n", str_ptr(token.u.funcao.id), tipo_dado_str(token.u.funcao.tipo));
      printf("Declaracao de funcao, Tipo: %s\n", tipo_dado_str(token.u.funcao.tipo));
      break;
    case DeclarationParameter:
      printf("Declaracao de parametro, Tipo: %s\n", tipo_dado_str(token.u.parametro.tipo));
      break;
    case DeclarationParameterList:
      printf("Declaracao de parametro em lista\n");
      break;
    case ParameterList:
      printf("Lista de parametros\n");
      break;
    case Block:
      printf("Bloco\n");
      break;
    case DeclarationVariable:
      printf("Declaracao de variavel, Tipo: %s\n", tipo_dado_str(token.u.type.tipo));
      break;
    case DeclarationList:
      printf("Lisa de declaracoes\n");
      break;
    case Assignment:
      printf("Atribuicai\n");
      break;
    case If:
      printf("If\n");
      break;
    case IfBlock:
      printf("IfBloco\n");
      break;
    case While:
      printf("While\n");
      break;
    case Return:
      printf("Return\n");
      break;
    case Print:
      printf("Print\n");
      break;
    case Read:
      printf("Read\n");
      break;
      break;
    case DeclarationFunctionList:
      printf("Lista de declaracao de funcoes\n");
      break;
    case FunctionCall:
      printf("Chamada de funcao\n");
      break;
    case ExpressionArithmetic:
      printf("Expressao Aritimetica, Operator: %d\n", token.u.arithmeticExpression.operator);
      break;
    case ExpressionRelational:
      printf("Expressao Relacional, Operator: %d\n", token.u.relationalExpression.operator);
      break;
    case ExpressionLogical:
      printf("Expressao Logica, Operator: %d\n", token.u.logicalExpression.operator);
      break;
    case Variable:
      printf("Variavel\n");
      break;
    case Type:
      printf("Tipo: %s\n", tipo_dado_str(token.u.type.tipo));
      break;
    case Literal:
      printf("Literal\n");
      break;
    case ConstantInt:
      printf("Constante Int: %d\n", token.u.constInt.valor);
      break;
    case ConstantFloat:
      printf("Constante Float: %f\n", token.u.constFloat.valor);
      break;
    case ConstantString:
      printf("Constante String: %s\n", str_ptr(token.u.constString.valor));
      break;
    case ConstantVoid:
      printf("Constante Void\n");
      break;
    case Identifier:
      printf("ID\n");
      break;
    case IdentifierList:
      printf("Lista de IDs\n");
      break;
    case CommandList:
      printf("Lista de comandos\n");
      break;
    default:
      printf("Desconhecido\n");
  }
}

void imprimir_traco(int nivel) {
  for (int i = 0; i < nivel; i++) printf("-");
}