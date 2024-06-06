#include "utils.h"

void imprimir_arvore_estilo_arvore(AST* raiz, int nivel) {
  if (raiz == NULL) return;

  imprimir_traco(nivel);

  switch (raiz->tipo) {
    case Arvore:
      imprimir_token(raiz->token, nivel);
      imprimir_traco(nivel + 1);
      printf("Arvore:\n");
      imprimir_arvore_estilo_arvore(raiz->u.arvore.left, nivel + 2);
      imprimir_arvore_estilo_arvore(raiz->u.arvore.right, nivel + 2);
      break;
    case Vetor:
      imprimir_token(raiz->token, nivel);
      imprimir_traco(nivel + 1);
      printf("Vetor:\n");
      for (AST** i = cvector_begin(raiz->u.filhos);
           i != cvector_end(raiz->u.filhos); i++) {
        imprimir_arvore_estilo_arvore(*i, nivel + 2);
      }
      break;
    case Folha:
      imprimir_token(raiz->token, nivel);
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
        case Constant: {
          switch (arvore->token.u.constante.tipo) {
            case Int: {
              printf("Constante {Int}: %d\n",
                     arvore->token.u.constante.valor.inteiro);
            } break;
            case Float: {
              printf("Constante {Float}: %f\n",
                     arvore->token.u.constante.valor.flutuante);
            } break;
            case String: {
              printf("Constante {String}: %s\n",
                     str_ptr(arvore->token.u.constante.valor.string));
            } break;
            case Void: {
              printf("Constante {Void}\n");
            } break;
            default:
              break;
          }
        } break;
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
        for (AST** i = cvector_begin(arvore->u.filhos);
             i != cvector_end(arvore->u.filhos); i++) {
          printar_folhas(*i);
        }
      }
      break;
  }
}

void imprimir_token(Token token, int nivel) {
  // imprimir_traco(nivel);

  switch (token.tipo) {
    case Program:
      printf("Programa\n");
      break;
    case DeclarationFunction:
      // printf("Declaracao de funcao, ID: %s, Type: %s\n",
      // str_ptr(token.u.funcao.id), tipo_dado_str(token.u.funcao.tipo));
      printf("Declaracao de funcao {%s}\n", tipo_dado_str(token.u.funcao.tipo));
      break;
    case DeclarationParameter:
      printf("Declaracao de parametro {%s}\n",
             tipo_dado_str(token.u.parametro.tipo));
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
      printf("Declaracao de variavel {%s}\n", tipo_dado_str(token.u.type.tipo));
      break;
    case DeclarationList:
      printf("Lisa de declaracoes\n");
      break;
    case Assignment:
      printf("Atribuicao\n");
      break;
    case If:
      printf("If\n");
      break;
    case IfBlock:
      printf("IfBloco\n");
      break;
    case ElseBlock:
      printf("ElseBloco\n");
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
      printf("Expressao Aritimetica {%s}\n",
             operador_aritmetico_str(token.u.arithmeticExpression.operator));
      break;
    case ExpressionRelational:
      printf("Expressao Relacional {%s}\n",
             operador_relacional_str(token.u.relationalExpression.operator));
      break;
    case ExpressionLogical:
      printf("Expressao Logica {%s}\n",
             operador_logico_str(token.u.logicalExpression.operator));
      break;
    case Variable:
      printf("Variavel\n");
      break;
    case Type:
      printf("Tipo {%s}\n", tipo_dado_str(token.u.type.tipo));
      break;
    case Literal:
      printf("Literal\n");
      break;
    case Constant: {
      switch (token.u.constante.tipo) {
        case Int: {
          printf("Constante {Int}: %d\n", token.u.constante.valor.inteiro);
        } break;
        case Float: {
          printf("Constante {Float}: %f\n", token.u.constante.valor.flutuante);
        } break;
        case String: {
          printf("Constante {String}: %s\n",
                 str_ptr(token.u.constante.valor.string));
        } break;
        case Void: {
          printf("Constante {Void}\n");
        } break;
        default:
          break;
      }
    } break;
    case Identifier:
      printf("ID {%s}\n", get_substring_before_delimiter(
                              str_ptr(token.u.idenfier.id), " {}();,"));
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
  printf("{%02d}", nivel);
  for (int i = 0; i < nivel; i++) printf("-");
}

char* operador_logico_str(enum LogicalOperators op) {
  switch (op) {
    case E:
      return "E(&&)";
    case Ou:
      return "Ou(||)";
    case Nao:
      return "Nao(!)";
    default:
      return "Unknown";
  }
}

char* operador_aritmetico_str(enum ArithmeticOperators op) {
  switch (op) {
    case Soma:
      return "Soma(+)";
    case Subtracao:
      return "Subtracao(-)";
    case Multiplicacao:
      return "Multiplicacao(*)";
    case Divisao:
      return "Divisao(/)";
    case Potencia:
      return "Potencia(^)";
    default:
      return "Unknown";
  }
}

char* operador_relacional_str(enum RelationsOperators op) {
  switch (op) {
    case Igual:
      return "==";
    case Diferente:
      return "!=";
    case Menor:
      return "<";
    case MenorIgual:
      return "<=";
    case Maior:
      return ">";
    case MaiorIgual:
      return ">=";
    default:
      return "Unknown";
  }
}

char* operador_logico_str_original(enum LogicalOperators op) {
  switch (op) {
    case E:
      return "&&";
    case Ou:
      return "||";
    case Nao:
      return "!";
    default:
      return "Unknown";
  }
}

char* operador_aritmetico_str_original(enum ArithmeticOperators op) {
  switch (op) {
    case Soma:
      return "+";
    case Subtracao:
      return "-";
    case Multiplicacao:
      return "*";
    case Divisao:
      return "/";
    case Potencia:
      return "^";
    default:
      return "Unknown";
  }
}

char* operador_relacional_str_original(enum RelationsOperators op) {
  switch (op) {
    case Igual:
      return "==";
    case Diferente:
      return "!=";
    case Menor:
      return "<";
    case MenorIgual:
      return "<=";
    case Maior:
      return ">";
    case MaiorIgual:
      return ">=";
    default:
      return "Unknown";
  }
}

char* tipo_dado_str_original(enum TipoDados tipo) {
  switch (tipo) {
    case Int:
      return "int";
    case Float:
      return "float";
    case String:
      return "string";
    case Void:
      return "void";
    default:
      return "Unknown";
  }
}

char* get_substring_before_delimiter(char* str, const char* delimiters) {
  char* substring = NULL;
  char* delimiter;
  size_t len;

  /* find the first occurrence of a delimiter in str */
  delimiter = strpbrk(str, delimiters);

  /* if a delimiter was found */
  if (delimiter != NULL) {
    /* calculate the length of the substring */
    len = delimiter - str;

    /* allocate memory for the substring */
    substring = malloc(len + 1);

    /* copy the substring from str to substring */
    strncpy(substring, str, len);

    /* add a null terminator to the end of the substring */
    substring[len] = '\0';
  }

  return substring;
}

void imprimir_codigo_original(AST* raiz) {
  if (!raiz) return;

  if (raiz) {
    switch (raiz->token.tipo) {
      case Program: {
        if (raiz->u.arvore.left) {
          imprimir_codigo_original(raiz->u.arvore.left);
        }

        printf("{");
        if (raiz->u.arvore.right) {
          imprimir_codigo_original(raiz->u.arvore.right);
        }
        printf("}");

      } break;
      case DeclarationFunction: {
        if (cvector_size(raiz->u.filhos) == 4) {
          AST* tipo = raiz->u.filhos[0];
          AST* id = raiz->u.filhos[1];
          AST* parametros = raiz->u.filhos[2];
          AST* bloco = raiz->u.filhos[3];

          printf("%s %s(", tipo_dado_str_original(tipo->token.u.type.tipo),
                 get_id_from_ID(id));
          if (parametros) {
            imprimir_codigo_original(parametros);
          }
          printf("){");
          if (bloco) {
            imprimir_codigo_original(bloco);
          }
          printf("}");

        } else if (cvector_size(raiz->u.filhos) == 3) {
          AST* tipo = raiz->u.filhos[0];
          AST* id = raiz->u.filhos[1];
          AST* bloco = raiz->u.filhos[2];

          printf("%s %s(", tipo_dado_str_original(tipo->token.u.type.tipo),
                 get_id_from_ID(id));
          printf("){");
          if (bloco) {
            imprimir_codigo_original(bloco);
          }
          printf("}");
        }
      } break;
      case DeclarationParameter: {
        AST* tipo = raiz->u.arvore.left;
        AST* id = raiz->u.arvore.right;

        printf("%s %s", tipo_dado_str_original(tipo->token.u.type.tipo),
               get_id_from_ID(id));
      } break;
      case DeclarationParameterList: {
        for (AST** i = cvector_begin(raiz->u.filhos);
             i != cvector_end(raiz->u.filhos); i++) {
          imprimir_codigo_original(*i);
        }
      } break;
      case ParameterList: {
        for (AST** i = cvector_begin(raiz->u.filhos);
             i != cvector_end(raiz->u.filhos); i++) {
          imprimir_codigo_original(*i);
          if (i != cvector_end(raiz->u.filhos) - 1) {
            printf(", ");
          }
        }
      } break;
      case Block: {
        AST* declaracoes = raiz->u.arvore.left;
        AST* comandos = raiz->u.arvore.right;

        if (declaracoes) {
          imprimir_codigo_original(declaracoes);
        }

        if (comandos) {
          imprimir_codigo_original(comandos);
        }
      } break;
      case DeclarationVariable: {
        AST* tipo = raiz->u.arvore.left;
        AST* id = raiz->u.arvore.right;

        printf("%s ", tipo_dado_str_original(tipo->token.u.type.tipo));
        imprimir_codigo_original(id);
        printf(";");
      } break;
      case DeclarationList: {
        for (AST** i = cvector_begin(raiz->u.filhos);
             i != cvector_end(raiz->u.filhos); i++) {
          imprimir_codigo_original(*i);
        }
      } break;
      case Assignment: {
        AST* id = raiz->u.arvore.left;
        AST* exp = raiz->u.arvore.right;

        printf("%s = ", get_id_from_ID(id));
        imprimir_codigo_original(exp);
        printf(";");
      }

      break;
      case If: {
        AST* condicao = raiz->u.arvore.left;

        printf("if (");
        imprimir_codigo_original(condicao);
        printf("){");
        if (raiz->u.arvore.right) {
          imprimir_codigo_original(raiz->u.arvore.right);
        }
      } break;
      case IfBlock: {
        AST* bloco = raiz->u.arvore.left;

        imprimir_codigo_original(bloco);

        printf("}");
        if (raiz->u.arvore.right) {
          printf("else{");
          imprimir_codigo_original(raiz->u.arvore.right);
          printf("}");
        }
      } break;
      case ElseBlock: {
        AST* bloco = raiz->u.arvore.left;

        imprimir_codigo_original(bloco);
      } break;
      case While: {
        AST* condicao = raiz->u.arvore.left;
        AST* bloco = raiz->u.arvore.right;

        printf("while(");
        imprimir_codigo_original(condicao);
        printf("){");
        if (bloco) {
          imprimir_codigo_original(bloco);
        }
        printf("}");

      } break;
      case Return: {
        AST* exp = raiz->u.arvore.left;

        printf("return ");
        imprimir_codigo_original(exp);
        printf(";");
      } break;
      case Print: {
        AST* exp = raiz->u.arvore.left;

        printf("print(");
        imprimir_codigo_original(exp);
        printf(");");
      } break;
      case Read: {
        AST* id = raiz->u.arvore.left;

        printf("read(%s);", get_id_from_ID(id));
      } break;
      case DeclarationFunctionList: {
        for (AST** i = cvector_begin(raiz->u.filhos);
             i != cvector_end(raiz->u.filhos); i++) {
          imprimir_codigo_original(*i);
        }
      } break;
      case FunctionCall: {
        AST* id = raiz->u.arvore.left;
        AST* parametros = raiz->u.arvore.right;

        printf("%s(", get_id_from_ID(id));
        if (parametros) {
          imprimir_codigo_original(parametros);
        }
        printf(");");
      } break;
      case ExpressionArithmetic: {
        AST* left = raiz->u.arvore.left;
        AST* right = raiz->u.arvore.right;

        if (left && left->tipo != Folha) {
          printf("(");
        }
        imprimir_codigo_original(left);
        if (left && left->tipo != Folha) {
          printf(")");
        }
        printf(" %s ", operador_aritmetico_str_original(
                           raiz->token.u.arithmeticExpression.operator));
        if (right && right->tipo != Folha) {
          printf("(");
        }
        imprimir_codigo_original(right);
        if (right && right->tipo != Folha) {
          printf(")");
        }
      } break;
      case ExpressionRelational: {
        AST* left = raiz->u.arvore.left;
        AST* right = raiz->u.arvore.right;

        if (left && left->tipo != Folha) {
          printf("(");
        }
        imprimir_codigo_original(left);
        if (left && left->tipo != Folha) {
          printf(")");
        }
        printf(" %s ", operador_relacional_str_original(
                           raiz->token.u.relationalExpression.operator));
        if (right && right->tipo != Folha) {
          printf("(");
        }
        imprimir_codigo_original(right);
        if (right && right->tipo != Folha) {
          printf(")");
        }
      } break;
      case ExpressionLogical: {
        AST* left = raiz->u.arvore.left;
        AST* right = raiz->u.arvore.right;

        if (left && left->tipo != Folha) {
          printf("(");
        }
        imprimir_codigo_original(left);
        if (left && left->tipo != Folha) {
          printf(")");
        }
        if (raiz->token.u.logicalExpression.operator== Nao) {
          printf("%s", operador_logico_str_original(
                           raiz->token.u.logicalExpression.operator));
        } else {
          printf(" %s ", operador_logico_str_original(
                             raiz->token.u.logicalExpression.operator));
        }
        if (right && right->tipo != Folha) {
          printf("(");
        }
        imprimir_codigo_original(right);
        if (right && right->tipo != Folha) {
          printf(")");
        }
      } break;
      case Variable: {
        AST* id = raiz->u.arvore.left;

        printf("%s", get_id_from_ID(id));
      } break;
      case Type: {
        printf("%s", tipo_dado_str_original(raiz->token.u.type.tipo));
      } break;
      case Literal: {
        AST* literal = raiz->u.arvore.left;

        printf("%s", get_id_from_ID(literal));
      } break;
      case Constant: {
        switch (raiz->token.u.constante.tipo) {
          case Int: {
            printf("%d", raiz->token.u.constante.valor.inteiro);
          } break;
          case Float: {
            printf("%f", raiz->token.u.constante.valor.flutuante);
          } break;
          case String: {
            printf("%s", str_ptr(raiz->token.u.constante.valor.string));
          } break;
          case Void: {
          } break;
          default:
            break;
        }
      } break;
      case Identifier: {
        printf("%s", get_id_from_ID(raiz));
      } break;
      case IdentifierList: {
        for (AST** i = cvector_begin(raiz->u.filhos);
             i != cvector_end(raiz->u.filhos); i++) {
          imprimir_codigo_original(*i);
          if (i != cvector_end(raiz->u.filhos) - 1) {
            printf(", ");
          }
        }
      } break;
      case CommandList: {
        for (AST** i = cvector_begin(raiz->u.filhos);
             i != cvector_end(raiz->u.filhos); i++) {
          imprimir_codigo_original(*i);
        }
      } break;
      default:
        printf("Desconhecido\n");
    }
  }
}

char* get_id_from_ID(AST* id) {
  return get_substring_before_delimiter(str_ptr(id->token.u.idenfier.id),
                                        " {}();,");
}

float intToFloat(int num) { return (float)num; }

str intToString(int num) {
  char* str = malloc((int)((ceil(log10(num)) + 1) * sizeof(char)));

  sprintf(str, "%d", num);

  return str_acquire(str);
}

float stringToFloat(str string) { return atof(str_ptr(string)); }

int stringToInt(str string) { return atoi(str_ptr(string)); }

int floatToInt(float num) { return (int)num; }

str floatToString(float num) {
  char* str = malloc((int)((ceil(log10(num)) + 1) * sizeof(char)));

  sprintf(str, "%f", num);

  return str_acquire(str);
}