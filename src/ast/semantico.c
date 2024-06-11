#include "ast.h"

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

void analise_semantica_funcoes(vector(AST *) declaracoes_funcoes) {
  for (AST **f = cvector_begin(declaracoes_funcoes); f != cvector_end(declaracoes_funcoes); f++) {
    AST *tipo = (*f)->u.filhos[0];
    analise_semantica_verificar_retorno_funcao(tipo->token.u.type.tipo, *f);
  }
}

void analise_semantica_verificar_retorno_funcao(enum TipoDados tipo, AST *bloco) {
  if (!bloco) return;

  if (bloco->token.tipo == Return) {
    AST *expressao = bloco->u.arvore.left;

    if (expressao->token.tipo == Constant) {
      if (expressao->token.u.constante.tipo != tipo) {
        converter_constant_para(expressao, tipo);
      }
    }
  }

  switch (bloco->tipo) {
    case Arvore:
      analise_semantica_verificar_retorno_funcao(tipo, bloco->u.arvore.left);
      analise_semantica_verificar_retorno_funcao(tipo, bloco->u.arvore.right);
      break;
    case Vetor:
      for (AST **it = cvector_begin(bloco->u.filhos); it != cvector_end(bloco->u.filhos); it++) {
        analise_semantica_verificar_retorno_funcao(tipo, *it);
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

          exibir_warning("Tipo inteiro nao eh equivalente a ponto flutuante!");
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

void analise_semantica_expressao(AST *arvore) {
  if (arvore) {
  }
}
