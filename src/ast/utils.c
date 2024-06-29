#include "ast.h"

void percorrer(AST *no, void (*fn)(AST *)) {
  if (no && fn) {
    fn(no);

    switch (no->tipo) {
      case Programa: {
        for (AST **it = cvector_begin(no->programa.funcoes); it != cvector_end(no->programa.funcoes); it++) {
          percorrer(*it, fn);
        }
        percorrer(no->programa.bloco, fn);
      } break;
      case Funcao: {
        for (AST **it = cvector_begin(no->funcao.parametros); it != cvector_end(no->funcao.parametros); it++) {
          percorrer(*it, fn);
        }
        percorrer(no->funcao.bloco, fn);

      } break;
      case Parametro: {
      } break;
      case Bloco: {
        for (AST **it = cvector_begin(no->bloco.declaracoes); it != cvector_end(no->bloco.declaracoes); it++) {
          percorrer(*it, fn);
        }

        for (AST **it = cvector_begin(no->bloco.comandos); it != cvector_end(no->bloco.comandos); it++) {
          percorrer(*it, fn);
        }

      } break;
      case Atribuicao: {
        percorrer(no->atribuicao.expressao, fn);

      } break;
      case If: {
        percorrer(no->if_.codicao, fn);

        for (AST **it = cvector_begin(no->if_.comandosIf); it != cvector_end(no->if_.comandosIf); it++) {
          percorrer(*it, fn);
        }

        for (AST **it = cvector_begin(no->if_.comandosElse); it != cvector_end(no->if_.comandosElse); it++) {
          percorrer(*it, fn);
        }

      } break;
      case While: {
        percorrer(no->while_.codicao, fn);
        for (AST **it = cvector_begin(no->while_.bloco); it != cvector_end(no->while_.bloco); it++) {
          percorrer(*it, fn);
        }

      } break;
      case Retorno: {
        percorrer(no->retorno.ret, fn);

      } break;
      case Print: {
      } break;
      case Read: {
      } break;
      case ChamadaFuncao: {
        for (AST **it = cvector_begin(no->chamada_funcao.parametros); it != cvector_end(no->chamada_funcao.parametros); it++) {
          percorrer(*it, fn);
        }

      } break;
      case ExpressaoRelacional: {
        percorrer(no->relacional.esquerda, fn);
        percorrer(no->relacional.direita, fn);

      } break;
      case ExpressaoLogica: {
        percorrer(no->logica.esquerda, fn);
        percorrer(no->logica.direita, fn);

      } break;
      case ExpressaoAritmetica: {
        percorrer(no->aritmetica.esquerda, fn);
        percorrer(no->aritmetica.direita, fn);

      } break;
      case ConsanteInt: {
      } break;
      case ConsanteFloat: {
      } break;
      case ConsanteString: {
      } break;
      case Id: {
      } break;
      case SituacaoTransicao: {
      } break;
      case Desconhecido: {
      } break;

      default:
        break;
    }
  }
}

const char *tipoToken_para_str(enum TipoToken token) {
  switch (token) {
    case Programa:
      return "Programa";
      break;
    case Funcao:
      return "Funcao";
      break;
    case Parametro:
      return "Parametro";
      break;
    case Bloco:
      return "Bloco";
      break;
    case Atribuicao:
      return "Atribuicao";
      break;
    case If:
      return "If";
      break;
    case While:
      return "While";
      break;
    case Retorno:
      return "Retorno";
      break;
    case Print:
      return "Print";
      break;
    case Read:
      return "Read";
      break;
    case ChamadaFuncao:
      return "Chamada de Funcao";
      break;
    case ExpressaoRelacional:
      return "Expressao Relacional";
      break;
    case ExpressaoLogica:
      return "Expressao Logica";
      break;
    case ExpressaoAritmetica:
      return "Expressao Aritmetica";
      break;
    case Tipo:
      return "Tipo";
      break;
    case ConsanteInt:
      return "Consante Int";
      break;
    case ConsanteFloat:
      return "Consante Flutuante";
      break;
    case ConsanteString:
      return "Consante String";
      break;
    case Id:
      return "Id";
      break;
    case Variavel:
      return "Variavel";
      break;
    case Desconhecido:
      return "Desconhecido";
      break;

    default:
      break;
  }

  return NULL;
}

void exibir_arvore(AST *no, int profundidade) {
  if (no) {
    printf("{%d}%s{%s}\n", profundidade, repeat_char(profundidade), tipoToken_para_str(no->tipo));

    switch (no->tipo) {
      case Programa: {
        for (AST **it = cvector_begin(no->programa.funcoes); it != cvector_end(no->programa.funcoes); it++) {
          exibir_arvore(*it, profundidade + 1);
        }
        exibir_arvore(no->programa.bloco, profundidade + 1);
      } break;
      case Funcao: {
        for (AST **it = cvector_begin(no->funcao.parametros); it != cvector_end(no->funcao.parametros); it++) {
          exibir_arvore(*it, profundidade + 1);
        }
        exibir_arvore(no->funcao.bloco, profundidade + 1);

      } break;
      case Bloco: {
        for (AST **it = cvector_begin(no->bloco.declaracoes); it != cvector_end(no->bloco.declaracoes); it++) {
          exibir_arvore(*it, profundidade + 1);
        }

        for (AST **it = cvector_begin(no->bloco.comandos); it != cvector_end(no->bloco.comandos); it++) {
          exibir_arvore(*it, profundidade + 1);
        }

      } break;
      case Atribuicao: {
        exibir_arvore(no->atribuicao.expressao, profundidade + 1);

      } break;
      case If: {
        exibir_arvore(no->if_.codicao, profundidade + 1);
        for (AST **it = cvector_begin(no->if_.comandosIf); it != cvector_end(no->if_.comandosIf); it++) {
          exibir_arvore(*it, profundidade + 1);
        }

        for (AST **it = cvector_begin(no->if_.comandosElse); it != cvector_end(no->if_.comandosElse); it++) {
          exibir_arvore(*it, profundidade + 1);
        }

      } break;
      case While: {
        exibir_arvore(no->while_.codicao, profundidade + 1);
        for (AST **it = cvector_begin(no->while_.bloco); it != cvector_end(no->while_.bloco); it++) {
          exibir_arvore(*it, profundidade + 1);
        }

      } break;
      case Retorno: {
        exibir_arvore(no->retorno.ret, profundidade + 1);

      } break;
      case ChamadaFuncao: {
        for (AST **it = cvector_begin(no->chamada_funcao.parametros); it != cvector_end(no->chamada_funcao.parametros); it++) {
          exibir_arvore(*it, profundidade + 1);
        }

      } break;
      case ExpressaoRelacional: {
        exibir_arvore(no->relacional.esquerda, profundidade + 1);
        exibir_arvore(no->relacional.direita, profundidade + 1);

      } break;
      case ExpressaoLogica: {
        exibir_arvore(no->logica.esquerda, profundidade + 1);
        exibir_arvore(no->logica.direita, profundidade + 1);

      } break;
      case ExpressaoAritmetica: {
        exibir_arvore(no->aritmetica.esquerda, profundidade + 1);
        exibir_arvore(no->aritmetica.direita, profundidade + 1);

      } break;

      default:
        break;
    }
  }
}

char *repeat_char(int n) {
  if (n <= 0) {
    return "";
  }

  char *str = malloc(n + 1);  // +1 for the null terminator
  if (!str) {
    return NULL;  // Failed to allocate memory
  }

  memset(str, '-', n);
  str[n] = '\0';  // Null terminate the string

  return str;
}

void constantInt_para_constantFloat(AST *constant) {
  if (constant && constant->tipo == ConsanteInt) {
    int aux = constant->inteiro;
    free(constant);
    constant = criar_ast(ConsanteFloat);
    constant->flutuante = (float)aux;
  }
}

void constantFloat_para_constantInt(AST *constant) {
  if (constant && constant->tipo == ConsanteFloat) {
    float aux = constant->flutuante;
    free(constant);
    constant = criar_ast(ConsanteInt);
    constant->inteiro = (int)aux;
  }
}

void expressaoAritmetica_para_Float(AST *expr) {
  if (expr && expr->tipo == ExpressaoAritmetica) {
    percorrer(expr, lambda(void, (AST * no), {
                if (no && no->tipo == ConsanteInt) {
                  constantInt_para_constantFloat(no);
                }
              }));
  }
}

void expressaoAritmetica_para_Int(AST *expr) {
  if (expr && expr->tipo == ExpressaoAritmetica) {
    percorrer(expr, lambda(void, (AST * no), {
                if (no && no->tipo == ConsanteFloat) {
                  constantFloat_para_constantInt(no);
                }
              }));
  }
}

enum TipoDado descobrir_tipo_expressao(AST *expr) {
  enum TipoDado tipo = Void;

  if (expr) {
    percorrer(expr, lambda(void, (AST * no), {
                if (no) {
                  if ((no->tipo == ConsanteInt || no->tipo == ConsanteFloat) && tipo != String) {
                    if (no->tipo == ConsanteFloat) {
                      tipo = Float;
                    } else if (no->tipo == ConsanteInt && (tipo != Float && tipo != String)) {
                      tipo = Int;
                    } else if (no->tipo == ConsanteString) {
                      exibir_erro("Proibido Strings em expressoes aritmeticas");
                      tipo = String;
                    }
                  }
                }
              }));
  }

  return tipo;
}

enum TipoDado descobrir_tipo_expressao_com_contexto(const char *contexto, AST *expr) {
  enum TipoDado tipo = Void;

  if (expr) {
    percorrer(expr, lambda(void, (AST * no), {
                if (no) {
                  if (no->tipo == ConsanteInt || no->tipo == ConsanteFloat) {
                    if (no->tipo == ConsanteFloat && tipo != String) {
                      tipo = Float;
                    } else if (no->tipo == ConsanteInt && (tipo != Float && tipo != String)) {
                      tipo = Int;
                    } else if (no->tipo == ConsanteString) {
                      exibir_erro("Proibido Strings em expressoes aritmeticas");
                      tipo = String;
                    }
                  } else if (no->tipo == Id) {
                    char msg[1000];
                    if (!id_sendo_usado_por_variavel(contexto, no->id)) {
                      sprintf(msg, "A variavel '%s' que esta num expressao aritmetica nao existe!", contexto);
                      exibir_erro(msg);
                    } else {
                      enum TipoDado tipo_var = get_tipo_dado_variavel(contexto, no->id);
                      if (tipo == Int && tipo_var == Float) {
                        tipo = Float;
                      } else if (tipo_var == String) {
                        sprintf(msg, "A variavel '%s' que esta numa expressao aritmetica tem tipo 'String' e isso eh proibido!", no->id);
                        exibir_erro(msg);
                        tipo = String;
                      }
                    }
                  } else if (no->tipo == ChamadaFuncao) {
                    char msg[1000];
                    if (!id_sendo_usado_por_funcao(no->chamada_funcao.id)) {
                      sprintf(msg, "A função '%s' chamada na expressão aritmetica não existe!", no->chamada_funcao.id);
                      exibir_erro(no->chamada_funcao.id);
                    } else {
                      enum TipoDado tipo_var = get_tipo_dado_funcao(no->chamada_funcao.id);
                      if (tipo == Int && tipo_var == Float) {
                        tipo = Float;
                      } else if (tipo_var == String) {
                        sprintf(msg, "A função chamada '%s' que esta numa expressao aritmetica tem tipo 'String' e isso eh proibido!",
                                no->id);
                        exibir_erro(msg);
                        tipo = String;
                      }
                    }
                  }
                }
              }));
  }

  return tipo;
}