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
          printf("DeclarationFunction, ID: %s, Type: %d\n", raiz->token.u.funcao.id, raiz->token.u.funcao.tipo);
          break;
        case DeclarationParameter:
          printf("DeclarationParameter, ID: %s, Type: %d\n", raiz->token.u.parametro.id, raiz->token.u.parametro.tipo);
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
          printf("DeclarationVariable, ID: %s, Type: %d\n", raiz->token.u.idenfier.id, raiz->token.u.type.tipo);
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
        case Function:
          printf("Function\n");
          break;
        case FunctionList:
          printf("FunctionList\n");
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
          printf("Type, Type: %d\n", raiz->token.u.type.tipo);
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
          printf("ConstantString: %s\n", raiz->token.u.constString.valor);
          break;
        case ConstantVoid:
          printf("ConstantVoid\n");
          break;
        case Identifier:
          printf("Identifier: %s\n", raiz->token.u.idenfier.id);
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