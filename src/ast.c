#include "ast.h"

AST *criar_programa(AST *funcoes, AST *blocoPrincipal) {
  AST *programa = (AST *)malloc(sizeof(AST));
  programa->tipo = Arvore;
  programa->token.tipo = Program;
  programa->u.arvore.left = funcoes;
  programa->u.arvore.right = blocoPrincipal;

  if (funcoes) {
    analise_semantica_funcoes(funcoes->u.filhos);
  }

  return programa;
}

AST *criar_lista_funcoes(AST *funcoes, AST *funcao) {
  if (funcoes == NULL) {
    funcoes = (AST *)malloc(sizeof(AST));
    funcoes->tipo = Vetor;
    funcoes->token.tipo = DeclarationFunctionList;
    funcoes->u.filhos = NULL;
  }
  cvector_push_back(funcoes->u.filhos, funcao);
  return funcoes;
}

AST *criar_parametro(AST *tipo, AST *id) {
  AST *parametro = (AST *)malloc(sizeof(AST));
  parametro->tipo = Arvore;
  parametro->token.tipo = DeclarationParameter;
  parametro->u.arvore.left = tipo;
  parametro->u.arvore.right = id;
  return parametro;
}

AST *criar_declaracao_parametros(AST *declaracaoAnterior, AST *parametro) {
  if (declaracaoAnterior == NULL) {
    declaracaoAnterior = (AST *)malloc(sizeof(AST));
    declaracaoAnterior->tipo = Vetor;
    declaracaoAnterior->token.tipo = DeclarationParameterList;
    declaracaoAnterior->u.filhos = NULL;
  }
  cvector_push_back(declaracaoAnterior->u.filhos, parametro);
  return declaracaoAnterior;
}

AST *criar_funcao(AST *tipo, AST *id, AST *parametros, AST *bloco) {
  AST *funcao = (AST *)malloc(sizeof(AST));
  funcao->tipo = Vetor;
  funcao->token.tipo = DeclarationFunction;
  funcao->u.filhos = NULL;
  cvector_push_back(funcao->u.filhos, tipo);
  cvector_push_back(funcao->u.filhos, id);
  cvector_push_back(funcao->u.filhos, parametros);
  cvector_push_back(funcao->u.filhos, bloco);
  return funcao;
}

AST *criar_funcao_input_void(AST *tipo, AST *id, AST *bloco) {
  AST *funcao = (AST *)malloc(sizeof(AST));
  funcao->tipo = Vetor;
  funcao->token.tipo = DeclarationFunction;
  funcao->u.filhos = NULL;
  cvector_push_back(funcao->u.filhos, tipo);
  cvector_push_back(funcao->u.filhos, id);
  cvector_push_back(funcao->u.filhos, bloco);
  return funcao;
}

AST *criar_tipo(enum TipoDados tipo) {
  AST *tipoAST = (AST *)malloc(sizeof(AST));
  tipoAST->tipo = Folha;
  tipoAST->token.tipo = Type;
  tipoAST->token.u.type.tipo = tipo;
  return tipoAST;
}

AST *criar_constante(const char *input, enum TipoDados tipo) {
  AST *constante = (AST *)malloc(sizeof(AST));
  constante->tipo = Folha;
  constante->token.tipo = Constant;

  switch (tipo) {
    case Int: {
      constante->token.u.constante.tipo = Int;
      constante->token.u.constante.valor.inteiro = atoi(input);
    } break;
    case Float: {
      constante->token.u.constante.tipo = Float;
      constante->token.u.constante.valor.flutuante = atof(input);
    } break;
    case String: {
      constante->token.u.constante.tipo = String;
      str_assign(&constante->token.u.constante.valor.string, str_acquire(input));
    } break;
    case Void: {
      constante->token.u.constante.tipo = Void;
      constante->token.u.constante.valor.nada = NULL;
    } break;

    default:
      break;
  }

  return constante;
}

AST *criar_idenfier(const char *input) {
  AST *idenfier = (AST *)malloc(sizeof(AST));
  idenfier->tipo = Folha;
  idenfier->token.tipo = Identifier;
  str_assign(&idenfier->token.u.idenfier.id, str_acquire(input));

  return idenfier;
}

AST *criar_bloco_principal(AST *declaracoes, AST *comandos) {
  AST *blocoPrincipal = (AST *)malloc(sizeof(AST));
  blocoPrincipal->token.tipo = Block;
  blocoPrincipal->tipo = Arvore;
  blocoPrincipal->u.arvore.left = declaracoes;
  blocoPrincipal->u.arvore.right = comandos;

  if (declaracoes && declaracoes->tipo == Vetor) {
    analise_semantica_variaveis(declaracoes->u.filhos, comandos);
  }

  return blocoPrincipal;
}

AST *criar_lista_identificadores(AST *identificadores, AST *id) {
  if (identificadores == NULL) {
    identificadores = (AST *)malloc(sizeof(AST));
    identificadores->tipo = Vetor;
    identificadores->token.tipo = IdentifierList;
    identificadores->u.filhos = NULL;
  }
  cvector_push_back(identificadores->u.filhos, id);
  return identificadores;
}

AST *criar_declaracao(AST *tipo, AST *ids) {
  AST *declaracao = (AST *)malloc(sizeof(AST));
  declaracao->tipo = Arvore;
  declaracao->token.tipo = DeclarationVariable;
  declaracao->u.arvore.left = tipo;
  declaracao->u.arvore.right = ids;
  return declaracao;
}

AST *criar_declaracoes(AST *declaracoes, AST *declaracao) {
  if (declaracoes == NULL) {
    declaracoes = (AST *)malloc(sizeof(AST));
    declaracoes->tipo = Vetor;
    declaracoes->token.tipo = DeclarationList;
    declaracoes->u.filhos = NULL;
  }
  cvector_push_back(declaracoes->u.filhos, declaracao);
  return declaracoes;
}

AST *criar_lista_comandos(AST *comandos, AST *comando) {
  if (comandos == NULL) {
    comandos = (AST *)malloc(sizeof(AST));
    comandos->tipo = Vetor;
    comandos->token.tipo = CommandList;
    comandos->u.filhos = NULL;
  }
  cvector_push_back(comandos->u.filhos, comando);
  return comandos;
}

AST *criar_comando_if(AST *condicao, AST *blocoIf, AST *blocoElse) {
  AST *comandoIf = (AST *)malloc(sizeof(AST));
  comandoIf->tipo = Arvore;
  comandoIf->token.tipo = If;
  comandoIf->u.arvore.left = condicao;
  comandoIf->u.arvore.right = (AST *)malloc(sizeof(AST));
  comandoIf->u.arvore.right->tipo = Arvore;
  comandoIf->u.arvore.right->token.tipo = If;
  comandoIf->u.arvore.right->u.arvore.left = blocoIf;
  comandoIf->u.arvore.right->u.arvore.right = blocoElse;

  // Dando nomes aos bois
  if (comandoIf->u.arvore.right->u.arvore.left) {
    comandoIf->u.arvore.right->u.arvore.left->token.tipo = IfBlock;
  }

  if (comandoIf->u.arvore.right->u.arvore.right) {
    comandoIf->u.arvore.right->u.arvore.right->token.tipo = ElseBlock;
  }
  return comandoIf;
}

AST *criar_comando_while(AST *condicao, AST *bloco) {
  AST *comandoWhile = (AST *)malloc(sizeof(AST));
  comandoWhile->tipo = Arvore;
  comandoWhile->token.tipo = While;
  comandoWhile->u.arvore.left = condicao;
  comandoWhile->u.arvore.right = bloco;

  return comandoWhile;
}

AST *criar_comando_atribuicao(AST *id, AST *expressao) {
  AST *comandoAtribuicao = (AST *)malloc(sizeof(AST));
  comandoAtribuicao->tipo = Arvore;
  comandoAtribuicao->token.tipo = Assignment;
  comandoAtribuicao->u.arvore.left = id;
  comandoAtribuicao->u.arvore.right = expressao;

  return comandoAtribuicao;
}

AST *criar_comando_read(AST *id) {
  AST *comandoRead = (AST *)malloc(sizeof(AST));
  comandoRead->tipo = Arvore;
  comandoRead->token.tipo = Read;
  comandoRead->u.arvore.left = id;
  comandoRead->u.arvore.right = NULL;

  return comandoRead;
}

AST *criar_comando_print(AST *expressao) {
  AST *comandoPrint = (AST *)malloc(sizeof(AST));
  comandoPrint->tipo = Arvore;
  comandoPrint->token.tipo = Print;
  comandoPrint->u.arvore.left = expressao;
  comandoPrint->u.arvore.right = NULL;

  return comandoPrint;
}

AST *criar_comando_chamada_funcao(AST *id, AST *parametros) {
  AST *comandoChamadaFuncao = (AST *)malloc(sizeof(AST));
  comandoChamadaFuncao->tipo = Arvore;
  comandoChamadaFuncao->token.tipo = FunctionCall;
  comandoChamadaFuncao->u.arvore.left = id;
  comandoChamadaFuncao->u.arvore.right = parametros;

  return comandoChamadaFuncao;
}

AST *criar_lista_parametros(AST *parametros, AST *parametro) {
  if (parametros == NULL) {
    parametros = (AST *)malloc(sizeof(AST));
    parametros->tipo = Vetor;
    parametros->token.tipo = ParameterList;
    parametros->u.filhos = NULL;
  }
  cvector_push_back(parametros->u.filhos, parametro);
  return parametros;
}

AST *criar_expressao_logica(enum LogicalOperators operator, AST * left, AST *right) {
  AST *expressaoLogica = (AST *)malloc(sizeof(AST));
  expressaoLogica->tipo = Arvore;
  expressaoLogica->token.tipo = ExpressionLogical;
  expressaoLogica->u.arvore.left = left;
  expressaoLogica->u.arvore.right = right;
  expressaoLogica->token.u.logicalExpression.operator= operator;
  return expressaoLogica;
}

AST *criar_expressao_aritmetica(enum ArithmeticOperators operator, AST * left, AST *right) {
  AST *expressaoAritmetica = (AST *)malloc(sizeof(AST));
  expressaoAritmetica->tipo = Arvore;
  expressaoAritmetica->token.tipo = ExpressionArithmetic;
  expressaoAritmetica->u.arvore.left = left;
  expressaoAritmetica->u.arvore.right = right;
  expressaoAritmetica->token.u.arithmeticExpression.operator= operator;
  return expressaoAritmetica;
}

AST *criar_expressao_relacional(enum RelationsOperators operator, AST * left, AST *right) {
  AST *expressaoRelacional = (AST *)malloc(sizeof(AST));
  expressaoRelacional->tipo = Arvore;
  expressaoRelacional->token.tipo = ExpressionRelational;
  expressaoRelacional->u.arvore.left = left;
  expressaoRelacional->u.arvore.right = right;
  expressaoRelacional->token.u.relationalExpression.operator= operator;
  return expressaoRelacional;
}

AST *criar_retorno_funcao(AST *expressao) {
  AST *retorno = (AST *)malloc(sizeof(AST));
  retorno->tipo = Arvore;
  retorno->token.tipo = Return;
  retorno->u.arvore.left = expressao ? expressao : criar_constante(NULL, Void);
  retorno->u.arvore.right = NULL;
  return retorno;
}

void analise_semantica_variaveis(vector(AST *) declaracoes_variaveis, AST *bloco) {
  if (!bloco) return;

  for (AST **item = cvector_begin(declaracoes_variaveis); item != cvector_end(declaracoes_variaveis); ++item) {
    enum TipoDados tipo = (*item)->u.arvore.left->token.u.type.tipo;
    vector(AST *) ids = (*item)->u.arvore.right->u.filhos;

    for (AST **id = cvector_begin(ids); id != cvector_end(ids); ++id) {
      analise_semantica_verificar_variavel((*id)->token.u.idenfier.id, tipo, bloco);
    }
  }
}

void analise_semantica_verificar_variavel(str id, enum TipoDados tipo, AST *ast) {
  if (!ast) return;
  switch (ast->tipo) {
    case Arvore: {
      if (ast->token.tipo == Assignment && str_eq(ast->u.arvore.left->token.u.idenfier.id, id) &&
          ast->u.arvore.right->token.u.constante.tipo != tipo) {
        converter_constant_para(ast->u.arvore.right, tipo);
      } else {
        analise_semantica_verificar_variavel(id, tipo, ast->u.arvore.left);
        analise_semantica_verificar_variavel(id, tipo, ast->u.arvore.right);
      }
    }

    break;
    case Vetor: {
      for (AST **it = cvector_begin(ast->u.filhos); it != cvector_end(ast->u.filhos); ++it) {
        analise_semantica_verificar_variavel(id, tipo, *it);
      }
    } break;
    case Folha: {
      return;
    };

    default:
      break;
  }
}

void printar_tipo_no(AST *no) {
  if (!no) {
    printf("No vazio/nulo\n");
  } else {
    switch (no->tipo) {
      case Arvore:
        printf("Arvore\n");
        break;
      case Vetor:
        printf("Vetor\n");

      case Folha:
        printf("Folha\n");
      default:
        break;
    }
  }
}

void analise_semantica_funcoes(vector(AST *) declaracoes_funcoes) {
  for (AST **f = cvector_begin(declaracoes_funcoes); f != cvector_end(declaracoes_funcoes); f++) {
    AST *tipo = (*f)->u.filhos[0];
    analise_semantica_verificar_funcao(tipo->token.u.type.tipo, *f);
  }
}

void analise_semantica_verificar_funcao(enum TipoDados tipo, AST *bloco) {
  if (!bloco) return;

  if (bloco->token.tipo == Return) {
    AST *expressao = bloco->u.arvore.left;

    if (expressao->token.tipo == Constant) {
      printf("Entrei\n");
      if (expressao->token.u.constante.tipo != tipo) {
        converter_constant_para(expressao, tipo);
      }
    }
  }

  switch (bloco->tipo) {
    case Arvore:
      analise_semantica_verificar_funcao(tipo, bloco->u.arvore.left);
      analise_semantica_verificar_funcao(tipo, bloco->u.arvore.right);
      break;
    case Vetor:
      for (AST **it = cvector_begin(bloco->u.filhos); it != cvector_end(bloco->u.filhos); it++) {
        analise_semantica_verificar_funcao(tipo, *it);
      }
      break;

    default:
      break;
  }
}

void converter_constant_para(AST *constante, enum TipoDados tipo) {
  if (!constante) return;

  switch (constante->token.u.constante.tipo) {
    case Int: {
      switch (tipo) {
        case Float: {
          constante->token.u.constante.tipo = Float;
          constante->token.u.constante.valor.flutuante = intToFloat(constante->token.u.constante.valor.inteiro);
        } break;
        case String: {
          constante->token.u.constante.tipo = String;
          constante->token.u.constante.valor.string = intToString(constante->token.u.constante.valor.inteiro);
        };

        default:
          break;
      }
    } break;
    case Float: {
      switch (tipo) {
        case Int: {
          constante->token.u.constante.tipo = Int;
          constante->token.u.constante.valor.inteiro = floatToInt(constante->token.u.constante.valor.flutuante);
        } break;
        case String: {
          constante->token.u.constante.tipo = String;
          constante->token.u.constante.valor.string = floatToString(constante->token.u.constante.valor.flutuante);
        };

        default:
          break;
      }
    } break;
    case String: {
      switch (constante->token.u.constante.tipo) {
        case Int: {
          constante->token.u.constante.tipo = Int;
          constante->token.u.constante.valor.inteiro = stringToInt(constante->token.u.constante.valor.string);
        } break;
        case Float: {
          constante->token.u.constante.tipo = Float;
          constante->token.u.constante.valor.flutuante = stringToFloat(constante->token.u.constante.valor.string);
        } break;

        default:
          break;
      }
    };

    default:
      break;
  }
}

void imprimir_arvore_estilo_arvore(AST *raiz, int nivel) {
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
      for (AST **i = cvector_begin(raiz->u.filhos); i != cvector_end(raiz->u.filhos); i++) {
        imprimir_arvore_estilo_arvore(*i, nivel + 2);
      }
      break;
    case Folha:
      imprimir_token(raiz->token, nivel);
      break;
  }
}

char *tipo_dado_str(enum TipoDados tipo) {
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

void printar_folhas(AST *arvore) {
  if (!arvore) return;

  switch (arvore->tipo) {
    case Folha:
      switch (arvore->token.tipo) {
        case Constant: {
          switch (arvore->token.u.constante.tipo) {
            case Int: {
              printf("Constante {Int}: %d\n", arvore->token.u.constante.valor.inteiro);
            } break;
            case Float: {
              printf("Constante {Float}: %f\n", arvore->token.u.constante.valor.flutuante);
            } break;
            case String: {
              printf("Constante {String}: %s\n", str_ptr(arvore->token.u.constante.valor.string));
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
        for (AST **i = cvector_begin(arvore->u.filhos); i != cvector_end(arvore->u.filhos); i++) {
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
      printf("Declaracao de parametro {%s}\n", tipo_dado_str(token.u.parametro.tipo));
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
      printf("Expressao Aritimetica {%s}\n", operador_aritmetico_str(token.u.arithmeticExpression.operator));
      break;
    case ExpressionRelational:
      printf("Expressao Relacional {%s}\n", operador_relacional_str(token.u.relationalExpression.operator));
      break;
    case ExpressionLogical:
      printf("Expressao Logica {%s}\n", operador_logico_str(token.u.logicalExpression.operator));
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
          printf("Constante {String}: %s\n", str_ptr(token.u.constante.valor.string));
        } break;
        case Void: {
          printf("Constante {Void}\n");
        } break;
        default:
          break;
      }
    } break;
    case Identifier:
      printf("ID {%s}\n", str_ptr(token.u.idenfier.id));
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

char *operador_logico_str(enum LogicalOperators op) {
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

char *operador_aritmetico_str(enum ArithmeticOperators op) {
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

char *operador_relacional_str(enum RelationsOperators op) {
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

char *operador_logico_str_original(enum LogicalOperators op) {
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

char *operador_aritmetico_str_original(enum ArithmeticOperators op) {
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

char *operador_relacional_str_original(enum RelationsOperators op) {
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

char *tipo_dado_str_original(enum TipoDados tipo) {
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

char *get_substring_before_delimiter(char *str, const char *delimiters) {
  char *substring = NULL;
  char *delimiter;
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

void imprimir_codigo_original(AST *raiz) {
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
          AST *tipo = raiz->u.filhos[0];
          AST *id = raiz->u.filhos[1];
          AST *parametros = raiz->u.filhos[2];
          AST *bloco = raiz->u.filhos[3];

          printf("%s %s(", tipo_dado_str_original(tipo->token.u.type.tipo), get_id_from_ID(id));
          if (parametros) {
            imprimir_codigo_original(parametros);
          }
          printf("){");
          if (bloco) {
            imprimir_codigo_original(bloco);
          }
          printf("}");

        } else if (cvector_size(raiz->u.filhos) == 3) {
          AST *tipo = raiz->u.filhos[0];
          AST *id = raiz->u.filhos[1];
          AST *bloco = raiz->u.filhos[2];

          printf("%s %s(", tipo_dado_str_original(tipo->token.u.type.tipo), get_id_from_ID(id));
          printf("){");
          if (bloco) {
            imprimir_codigo_original(bloco);
          }
          printf("}");
        }
      } break;
      case DeclarationParameter: {
        AST *tipo = raiz->u.arvore.left;
        AST *id = raiz->u.arvore.right;

        printf("%s %s", tipo_dado_str_original(tipo->token.u.type.tipo), get_id_from_ID(id));
      } break;
      case DeclarationParameterList: {
        for (AST **i = cvector_begin(raiz->u.filhos); i != cvector_end(raiz->u.filhos); i++) {
          imprimir_codigo_original(*i);
        }
      } break;
      case ParameterList: {
        for (AST **i = cvector_begin(raiz->u.filhos); i != cvector_end(raiz->u.filhos); i++) {
          imprimir_codigo_original(*i);
          if (i != cvector_end(raiz->u.filhos) - 1) {
            printf(", ");
          }
        }
      } break;
      case Block: {
        AST *declaracoes = raiz->u.arvore.left;
        AST *comandos = raiz->u.arvore.right;

        if (declaracoes) {
          imprimir_codigo_original(declaracoes);
        }

        if (comandos) {
          imprimir_codigo_original(comandos);
        }
      } break;
      case DeclarationVariable: {
        AST *tipo = raiz->u.arvore.left;
        AST *id = raiz->u.arvore.right;

        printf("%s ", tipo_dado_str_original(tipo->token.u.type.tipo));
        imprimir_codigo_original(id);
        printf(";");
      } break;
      case DeclarationList: {
        for (AST **i = cvector_begin(raiz->u.filhos); i != cvector_end(raiz->u.filhos); i++) {
          imprimir_codigo_original(*i);
        }
      } break;
      case Assignment: {
        AST *id = raiz->u.arvore.left;
        AST *exp = raiz->u.arvore.right;

        printf("%s = ", get_id_from_ID(id));
        imprimir_codigo_original(exp);
        printf(";");
      }

      break;
      case If: {
        AST *condicao = raiz->u.arvore.left;

        printf("if (");
        imprimir_codigo_original(condicao);
        printf("){");
        if (raiz->u.arvore.right) {
          imprimir_codigo_original(raiz->u.arvore.right);
        }
      } break;
      case IfBlock: {
        AST *bloco = raiz->u.arvore.left;

        imprimir_codigo_original(bloco);

        printf("}");
        if (raiz->u.arvore.right) {
          printf("else{");
          imprimir_codigo_original(raiz->u.arvore.right);
          printf("}");
        }
      } break;
      case ElseBlock: {
        AST *bloco = raiz->u.arvore.left;

        imprimir_codigo_original(bloco);
      } break;
      case While: {
        AST *condicao = raiz->u.arvore.left;
        AST *bloco = raiz->u.arvore.right;

        printf("while(");
        imprimir_codigo_original(condicao);
        printf("){");
        if (bloco) {
          imprimir_codigo_original(bloco);
        }
        printf("}");

      } break;
      case Return: {
        AST *exp = raiz->u.arvore.left;

        printf("return ");
        imprimir_codigo_original(exp);
        printf(";");
      } break;
      case Print: {
        AST *exp = raiz->u.arvore.left;

        printf("print(");
        imprimir_codigo_original(exp);
        printf(");");
      } break;
      case Read: {
        AST *id = raiz->u.arvore.left;

        printf("read(%s);", get_id_from_ID(id));
      } break;
      case DeclarationFunctionList: {
        for (AST **i = cvector_begin(raiz->u.filhos); i != cvector_end(raiz->u.filhos); i++) {
          imprimir_codigo_original(*i);
        }
      } break;
      case FunctionCall: {
        AST *id = raiz->u.arvore.left;
        AST *parametros = raiz->u.arvore.right;

        printf("%s(", get_id_from_ID(id));
        if (parametros) {
          imprimir_codigo_original(parametros);
        }
        printf(");");
      } break;
      case ExpressionArithmetic: {
        AST *left = raiz->u.arvore.left;
        AST *right = raiz->u.arvore.right;

        if (left && left->tipo != Folha) {
          printf("(");
        }
        imprimir_codigo_original(left);
        if (left && left->tipo != Folha) {
          printf(")");
        }
        printf(" %s ", operador_aritmetico_str_original(raiz->token.u.arithmeticExpression.operator));
        if (right && right->tipo != Folha) {
          printf("(");
        }
        imprimir_codigo_original(right);
        if (right && right->tipo != Folha) {
          printf(")");
        }
      } break;
      case ExpressionRelational: {
        AST *left = raiz->u.arvore.left;
        AST *right = raiz->u.arvore.right;

        if (left && left->tipo != Folha) {
          printf("(");
        }
        imprimir_codigo_original(left);
        if (left && left->tipo != Folha) {
          printf(")");
        }
        printf(" %s ", operador_relacional_str_original(raiz->token.u.relationalExpression.operator));
        if (right && right->tipo != Folha) {
          printf("(");
        }
        imprimir_codigo_original(right);
        if (right && right->tipo != Folha) {
          printf(")");
        }
      } break;
      case ExpressionLogical: {
        AST *left = raiz->u.arvore.left;
        AST *right = raiz->u.arvore.right;

        if (left && left->tipo != Folha) {
          printf("(");
        }
        imprimir_codigo_original(left);
        if (left && left->tipo != Folha) {
          printf(")");
        }
        if (raiz->token.u.logicalExpression.operator== Nao) {
          printf("%s", operador_logico_str_original(raiz->token.u.logicalExpression.operator));
        } else {
          printf(" %s ", operador_logico_str_original(raiz->token.u.logicalExpression.operator));
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
        AST *id = raiz->u.arvore.left;

        printf("%s", get_id_from_ID(id));
      } break;
      case Type: {
        printf("%s", tipo_dado_str_original(raiz->token.u.type.tipo));
      } break;
      case Literal: {
        AST *literal = raiz->u.arvore.left;

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
        for (AST **i = cvector_begin(raiz->u.filhos); i != cvector_end(raiz->u.filhos); i++) {
          imprimir_codigo_original(*i);
          if (i != cvector_end(raiz->u.filhos) - 1) {
            printf(", ");
          }
        }
      } break;
      case CommandList: {
        for (AST **i = cvector_begin(raiz->u.filhos); i != cvector_end(raiz->u.filhos); i++) {
          imprimir_codigo_original(*i);
        }
      } break;
      default:
        printf("Desconhecido\n");
    }
  }
}

char *get_id_from_ID(AST *id) { return str_ptr(id->token.u.idenfier.id); }