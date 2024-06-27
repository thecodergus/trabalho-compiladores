#include "ast.h"

void percorrer(AST *no, void (*fn)(AST *)) {
  if (no && fn) {
    fn(no);

    switch (no->tipo) {
    case Programa: {
      for (AST **it = cvector_begin(no->programa.funcoes);
           it != cvector_end(no->programa.funcoes); it++) {
        percorrer(*it, fn);
      }
      percorrer(no->programa.bloco, fn);
    } break;
    case Funcao: {
      for (AST **it = cvector_begin(no->funcao.parametros);
           it != cvector_end(no->funcao.parametros); it++) {
        percorrer(*it, fn);
      }
      percorrer(no->funcao.bloco, fn);

    } break;
    case Parametro: {

    } break;
    case Bloco: {
      for (AST **it = cvector_begin(no->bloco.comandos);
           it != cvector_end(no->bloco.comandos); it++) {
        percorrer(*it, fn);
      }

      for (AST **it = cvector_begin(no->bloco.declaracoes);
           it != cvector_end(no->bloco.declaracoes); it++) {
        percorrer(*it, fn);
      }

    } break;
    case Atribuicao: {
      percorrer(no->atribuicao.expressao, fn);

    } break;
    case If: {

      for (AST **it = cvector_begin(no->if_.blocoIf);
           it != cvector_end(no->if_.blocoIf); it++) {
        percorrer(*it, fn);
      }

      for (AST **it = cvector_begin(no->if_.blocoElse);
           it != cvector_end(no->if_.blocoElse); it++) {
        percorrer(*it, fn);
      }

    } break;
    case While: {

      for (AST **it = cvector_begin(no->while_.bloco);
           it != cvector_end(no->while_.bloco); it++) {
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

      for (AST **it = cvector_begin(no->chamada_funcao.parametros);
           it != cvector_end(no->chamada_funcao.parametros); it++) {
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
    return "Situacao de Transicao";
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

void exibir_arvore(AST *no) {

  if (no) {
    int profundidade = 0;
    percorrer(no, lambda(void, (AST * no_), {
                if (no_) {
                  printf("{%d}{%s}\n", profundidade++,
                         tipoToken_para_str(no_->tipo));
                }
              }));
  }
}