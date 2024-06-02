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
          printf("DeclarationFunction, ID: %s, Type: %s\n", str_ptr(raiz->token.u.funcao.id), tipo_dado_str(raiz->token.u.funcao.tipo));
          break;
        case DeclarationParameter:
          printf("DeclarationParameter, ID: %s, Type: %s\n", str_ptr(raiz->token.u.parametro.id),
                 tipo_dado_str(raiz->token.u.parametro.tipo));
          break;
        case DeclarationParameterList:
          printf("DeclarationParameterList\n");
          break;
        case ParameterList:
          printf("ParameterList\n");
          break;
        case Block:
          printf("Block\n");
          break;
        case DeclarationVariable:
          printf("DeclarationVariable, ID: %s, Type: %sS\n", str_ptr(raiz->token.u.idenfier.id), tipo_dado_str(raiz->token.u.type.tipo));
          break;
        case DeclarationList:
          printf("DeclarationList\n");
          break;
        case Assignment:
          printf("Assignment\n");
          break;
        case If:
          printf("If\n");
          break;
        case IfBlock:
          printf("IfBlock\n");
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
          printf("DeclarationFunctionList\n");
          break;
        case FunctionCall:
          printf("FunctionCall\n");
          break;
        case ExpressionArithmetic:
          printf("ArithmeticExpression, Operator: %d\n", raiz->token.u.arithmeticExpression.operator);
          break;
        case ExpressionRelational:
          printf("RelationalExpression, Operator: %d\n", raiz->token.u.relationalExpression.operator);
          break;
        case ExpressionLogical:
          printf("LogicalExpression, Operator: %d\n", raiz->token.u.logicalExpression.operator);
          break;
        case Variable:
          printf("Variable\n");
          break;
        case Type:
          printf("Type, Type: %s\n", tipo_dado_str(raiz->token.u.type.tipo));
          break;
        case Literal:
          printf("Literal\n");
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
        case ConstantVoid:
          printf("ConstantVoid\n");
          break;
        case Identifier:
          printf("Identifier: %s\n", str_ptr(raiz->token.u.idenfier.id));
          break;
        case IdentifierList:
          printf("IdentifierList\n");
          break;
        case CommandList:
          printf("CommandList\n");
          break;
        default:
          printf("Unknown\n");
      }

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
      if (!arvore->u.arvore.left) printar_folhas(arvore->u.arvore.left);
      if (!arvore->u.arvore.right) printar_folhas(arvore->u.arvore.right);
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